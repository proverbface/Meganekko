/* 
 * Copyright 2015 eje inc.
 * Copyright 2015 Samsung Electronics Co., LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.eje_c.meganekko.gearvr;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.KeyEvent;

import com.eje_c.meganekko.Frame;
import com.eje_c.meganekko.Meganekko;
import com.eje_c.meganekko.MeganekkoApp;
import com.eje_c.meganekko.utility.DockEventReceiver;
import com.oculus.vrappframework.VrActivity;

import ovr.App;
import ovr.VrFrame;

/**
 * The Meganekko application will have a single Android {@link Activity} which extends {@link MeganekkoActivity},
 * not directly from {@code Activity}. {@code MeganekkoActivity} creates and manages the internal classes which use
 * sensor data to manage a viewpoint, and thus present an appropriate stereoscopic view of your scene graph.
 */
public abstract class MeganekkoActivity extends VrActivity implements Meganekko {

    static {
        System.loadLibrary("meganekko");
    }

    private InternalSensorManager mInternalSensorManager;
    private boolean mDocked;
    private final Runnable mRunOnDock = new Runnable() {
        @Override
        public void run() {
            mDocked = true;
            mInternalSensorManager.stop();
        }
    };
    private final Runnable mRunOnUndock = new Runnable() {
        @Override
        public void run() {
            mDocked = false;
        }
    };
    private App mApp;
    private DockEventReceiver mDockEventReceiver;
    private MeganekkoApp meganekkoApp;

    private static native long nativeSetAppInterface(VrActivity act, String fromPackageName, String commandString, String uriString);

    private static native void nativeHideGazeCursor(long appPtr);

    private static native void nativeShowGazeCursor(long appPtr);

    public static native void setDebugOptionEnable(boolean enable);

    private static native void recenterPose(long appPtr);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Intent intent = getIntent();
        String commandString = getCommandStringFromIntent(intent);
        String fromPackageNameString = getPackageStringFromIntent(intent);
        String uriString = getUriStringFromIntent(intent);

        setAppPtr(nativeSetAppInterface(this, fromPackageNameString, commandString, uriString));

        mDockEventReceiver = new DockEventReceiver(this, mRunOnDock, mRunOnUndock);
        mInternalSensorManager = new InternalSensorManager(this, getAppPtr());

        mApp = new App(getAppPtr());
    }

    @Override
    protected void onPause() {
        super.onPause();
        mDockEventReceiver.stop();

        if (meganekkoApp != null) {
            meganekkoApp.onPause();
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        mDockEventReceiver.start();

        if (meganekkoApp != null) {
            meganekkoApp.onResume();
        }
    }

    /**
     * Called from native AppInterface::oneTimeInit().
     */
    private void oneTimeInit() {

        meganekkoApp = createMeganekkoApp(this);

        if (!mDocked) {
            mInternalSensorManager.start();
        }
    }

    /**
     * Called from native AppInterface::frame().
     */
    private void frame(long vrFramePtr) {

        // Setup VrFrame
        Frame vrFrame = meganekkoApp.getFrame();
        if (vrFrame == null) {
            vrFrame = new VrFrame(vrFramePtr);
            meganekkoApp.setFrame(vrFrame);
        }

        meganekkoApp.update();
    }

    /**
     * Called from native AppInterface::oneTimeShutDown().
     */
    private void oneTimeShutDown() {

        meganekkoApp.shutdown();

        if (!mDocked) {
            mInternalSensorManager.stop();
        }
    }

    /**
     * Called from native AppInterface::EnteredVrMode().
     */
    protected void enteredVrMode() {
    }

    /**
     * Called from native AppInterface::LeavingVrMode().
     */
    protected void leavingVrMode() {
    }

    protected void onHmdMounted() {
    }

    protected void onHmdUnmounted() {
    }

    public void hideGazeCursor() {
        nativeHideGazeCursor(getAppPtr());
    }

    public void showGazeCursor() {
        nativeShowGazeCursor(getAppPtr());
    }

    public boolean onKeyShortPress(int keyCode, int repeatCount) {
        return meganekkoApp.onKeyShortPress(keyCode, repeatCount);
    }

    public boolean onKeyDoubleTap(int keyCode, int repeatCount) {
        return meganekkoApp.onKeyDoubleTap(keyCode, repeatCount);
    }

    public boolean onKeyLongPress(int keyCode, int repeatCount) {
        return meganekkoApp.onKeyLongPress(keyCode, repeatCount);
    }

    public boolean onKeyDown(int keyCode, int repeatCount) {
        return meganekkoApp.onKeyDown(keyCode, repeatCount);
    }

    public boolean onKeyUp(int keyCode, int repeatCount) {
        return meganekkoApp.onKeyUp(keyCode, repeatCount);
    }

    public boolean onKeyMax(int keyCode, int repeatCount) {
        return meganekkoApp.onKeyMax(keyCode, repeatCount);
    }

    @Deprecated
    @Override
    public boolean onKeyLongPress(int keyCode, KeyEvent event) {
        return super.onKeyLongPress(keyCode, event);
    }

    @Deprecated
    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
        return super.onKeyUp(keyCode, event);
    }

    @Deprecated
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        return super.onKeyDown(keyCode, event);
    }

    @Override
    public void recenter() {
        recenterPose(getAppPtr());
    }

    public App getApp() {
        return mApp;
    }

    @Override
    public Context getContext() {
        return this;
    }

    private long getNativeScene() {
        return meganekkoApp.getScene().getNative();
    }
}