package com.google.cloud.android.speech;

import android.content.Context;
import android.support.v4.app.Fragment;
import android.util.Log;

import com.google.cloud.android.speech.Global;
import com.google.cloud.android.speech.RobotSpeak;


/*
所有的fragment請繼承這個fragment,要跳轉到下一個fragment請call jumpNextFragment
 */
public abstract class SpeechFragment extends Fragment {

    public abstract SpeechReaction getSpeechReaction();

    @Override
    public void onAttach(Context context) {
        super.onAttach(context);
        Log.d("SpeechFragment", "onAttach");
        Global.mainAct.setCurrentReaction(getSpeechReaction());
    }


    public void jumpNextFragment(Fragment f) {
        // stop talking before next fragment
        if (Global.api != null){
            RobotSpeak.stopTalking();
        }

        // clear character before next fragment (uncomment if needed)
//        Global.character = null;

        // next fragment
        Global.mainAct.changeFragment(f);
    }
}
