/*
 * Copyright 2015 eje inc.
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
package com.eje_c.meganekko.xml;

import android.content.Context;

/**
 * Simple singleton implementation for {@link XmlSceneParser}.
 */
public class XmlSceneParserFactory {

    private static XmlSceneParserFactory instance;
    private final Context mContext;
    private XmlSceneParser mSceneParser;

    public XmlSceneParserFactory(Context context) {
        mContext = context;
    }

    /**
     * Get a singleton instance of {@link XmlSceneParserFactory}.
     *
     * @return Singleton instance.
     */
    public synchronized static XmlSceneParserFactory getInstance(Context context) {
        if (instance == null) {
            instance = new XmlSceneParserFactory(context.getApplicationContext());
        }
        return instance;
    }

    /**
     * Get a singleton instance of {@link XmlSceneParser}.
     *
     * @return Singleton instance.
     */
    public synchronized XmlSceneParser getSceneParser() {
        if (mSceneParser == null) {
            mSceneParser = new XmlSceneParser(mContext);
        }
        return mSceneParser;
    }
}
