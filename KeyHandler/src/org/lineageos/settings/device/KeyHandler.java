/*
 * Copyright (C) 2018 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.lineageos.settings.device;

import android.content.Context;
import android.media.AudioManager;
import android.os.VibrationEffect;
import android.os.Vibrator;
import android.view.KeyEvent;

import com.android.internal.os.DeviceKeyHandler;

public class KeyHandler implements DeviceKeyHandler {
    private static final String TAG = KeyHandler.class.getSimpleName();

    // Slider key codes
    private static final int MODE_NORMAL = 601;
    private static final int MODE_VIBRATION = 602;
    private static final int MODE_SILENCE = 603;

    // Vibration effects
    private static final VibrationEffect MODE_NORMAL_EFFECT =
            VibrationEffect.createOneShot(250, VibrationEffect.DEFAULT_AMPLITUDE);
    private static final VibrationEffect MODE_VIBRATION_EFFECT =
            VibrationEffect.get(VibrationEffect.EFFECT_DOUBLE_CLICK);

    private final Context mContext;
    private final AudioManager mAudioManager;
    private final Vibrator mVibrator;

    public KeyHandler(Context context) {
        mContext = context;

        mAudioManager = mContext.getSystemService(AudioManager.class);
        mVibrator = mContext.getSystemService(Vibrator.class);
    }

    public KeyEvent handleKeyEvent(KeyEvent event) {
        int scanCode = event.getScanCode();

        switch (scanCode) {
            case MODE_NORMAL:
                mAudioManager.setRingerModeInternal(AudioManager.RINGER_MODE_NORMAL);
                doHapticFeedback(MODE_NORMAL_EFFECT);
                break;
            case MODE_VIBRATION:
                mAudioManager.setRingerModeInternal(AudioManager.RINGER_MODE_VIBRATE);
                doHapticFeedback(MODE_VIBRATION_EFFECT);
                break;
            case MODE_SILENCE:
                mAudioManager.setRingerModeInternal(AudioManager.RINGER_MODE_SILENT);
                break;
            default:
                return event;
        }

        return null;
    }

    private void doHapticFeedback(VibrationEffect effect) {
        if (mVibrator != null && mVibrator.hasVibrator()) {
            mVibrator.vibrate(effect);
        }
    }
}
