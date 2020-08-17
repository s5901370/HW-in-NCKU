package com.google.cloud.android.speech;


import android.os.Bundle;
import android.support.constraint.ConstraintLayout;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

import com.asus.robotframework.API.RobotFace;

import java.util.Timer;
import java.util.TimerTask;

import com.google.cloud.android.speech.R;
import com.google.cloud.android.speech.Global;
import com.google.cloud.android.speech.SpeechFragment;
import com.google.cloud.android.speech.SpeechReaction;
import com.google.cloud.android.speech.DebugLogger;
import com.google.cloud.android.speech.TextUtil;


public class HEQuestionFragment extends SpeechFragment {

    // UI
    private ImageButton mHomeBtn;
    private ImageButton mNextBtn;
    private TextView mQuestionTextView, mSubTypeTextView;
    private ConstraintLayout mConstraintLayout;

    // for health edu
    private String mQuestionText, mSubTypeText;
    private String mOptionA, mOptionB, mOptionC, mOptionD;
    private String mAnswer;
    private String mState;


    public HEQuestionFragment() {
        // Required empty public constructor
    }


    public static HEQuestionFragment newInstance() {
        HEQuestionFragment fragment = new HEQuestionFragment();
        Bundle args = new Bundle();
        fragment.setArguments(args);
        return fragment;
    }

    public static HEQuestionFragment newInstance(Bundle args) {
        HEQuestionFragment fragment = new HEQuestionFragment();
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (getArguments() != null) {
            mQuestionText = getArguments().getString("questionText");
            mSubTypeText = getArguments().getString("subtypeText");
            mAnswer = getArguments().getString("answer");
            mState = getArguments().getString("state");
            mOptionA = getArguments().getString("A");
            mOptionB = getArguments().getString("B");

            try {
                mOptionC = getArguments().getString("C");
                mOptionD = getArguments().getString("D");

                Log.d("HEQuestion", "Have C and D");
            }catch (NullPointerException e){
                Log.d("HEQuestion", "No C and D");
            }
        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View view = inflater.inflate(R.layout.fragment_hequestion, container, false);

        initView(view);
        return view;
    }

    private void initView(View view) {
        Global.api.robot.stopSpeak();

        mConstraintLayout = view.findViewById(R.id.rootConstraintLayout);
        mConstraintLayout.setOnClickListener(v ->{
            Log.d("HEQuestion", "ConstraintLayout onclick");
            Bundle bundle = new Bundle();
            bundle.putString("questionText", mQuestionText);
            bundle.putString("subtypeText", mSubTypeText);
            bundle.putString("answer", mAnswer);
            bundle.putString("state", mState);
            bundle.putString("A", mOptionA);
            bundle.putString("B", mOptionB);
            try {
                if (!mOptionC.equals("")) {
                    bundle.putString("C", mOptionC);
                }
                if (!mOptionD.equals("")) {
                    bundle.putString("D", mOptionD);
                }
                jumpNextFragment(HEOptionFragment.newInstance(bundle));
            }catch (NullPointerException e){
                Log.d("HEQuestion", "No C and D");
                jumpNextFragment(HEOptionFragment.newInstance(bundle));
            }

            jumpNextFragment(HEOptionFragment.newInstance(bundle));
        });

        mHomeBtn = view.findViewById(R.id.homeButton);
        mHomeBtn.setOnClickListener(v -> {
            jumpNextFragment(new MainActivityFragment());
        });

        mNextBtn = view.findViewById(R.id.nextButton);
        mNextBtn.setOnClickListener(v -> {
            Log.d("HEQuestion", "mNextBtn onclick");
            Bundle bundle = new Bundle();
            bundle.putString("questionText", mQuestionText);
            bundle.putString("subtypeText", mSubTypeText);
            bundle.putString("answer", mAnswer);
            bundle.putString("state", mState);
            bundle.putString("A", mOptionA);
            bundle.putString("B", mOptionB);
            try {
                if (!mOptionC.equals("")) {
                    bundle.putString("C", mOptionC);
                }
                if (!mOptionD.equals("")) {
                    bundle.putString("D", mOptionD);
                }
                jumpNextFragment(HEOptionFragment.newInstance(bundle));
            }catch (NullPointerException e){
                Log.d("HEQuestion", "No C and D");
                jumpNextFragment(HEOptionFragment.newInstance(bundle));
            }

            jumpNextFragment(HEOptionFragment.newInstance(bundle));

        });

        mQuestionTextView = view.findViewById(R.id.questionText);
        mQuestionTextView.setText(mQuestionText);
        Global.api.robot.speak(mQuestionText);

        mSubTypeTextView = view.findViewById(R.id.subType);
        mSubTypeTextView.setText(mSubTypeText);
        switch (mSubTypeText){
            case "問題解決":
                mSubTypeTextView.setBackgroundResource(R.color.problem);
                break;
            case "健康飲食":
                mSubTypeTextView.setBackgroundResource(R.color.diet);
                break;
            case "藥物治療":
                mSubTypeTextView.setBackgroundResource(R.color.drug);
                break;
            case "降低風險":
                mSubTypeTextView.setBackgroundResource(R.color.risk);
                break;
            case "血糖監測":
                mSubTypeTextView.setBackgroundResource(R.color.blood);
                break;
            case "體能活動":
                mSubTypeTextView.setBackgroundResource(R.color.fitness);
                break;
            case "健康認知":
                mSubTypeTextView.setBackgroundResource(R.color.recog);
                break;
            default:
                //mSubTypeTextView.setBackgroundResource(R.color.problem);
        }

    }


    @Override
    public SpeechReaction getSpeechReaction() {
        return new SpeechReaction() {
            @Override
            public void onSentenceEnd(String text) {
                String[] keywordsOfStart = {"開始", "go", "start", "begin",
                        "推薦", "故事", "歌曲", "story", "recommendation", "song", "recommend", "主畫面"};
                String[] keywordsOfOver = {"結束", "回去", "上一頁", "首頁", "謝謝你", "over", "back", "home", "thank you"};
                String[] keywordsOfRecommend = {"推薦", "故事", "歌曲", "story", "recommendation", "song", "recommend"};
                String[] keywordsOfNext = {"下面", "下一頁", "選項", "看", "next", "page"};

                if (TextUtil.inKeywords(text, keywordsOfStart) || TextUtil.inKeywords(text, keywordsOfRecommend)) {
                    jumpNextFragment(new MainActivityFragment());        // TODO: Need to define where to go
                }else if(TextUtil.inKeywords(text, keywordsOfOver)){
                    mHomeBtn.performClick();
                }else if(TextUtil.inKeywords(text, keywordsOfNext)){
                    mNextBtn.performClick();
                }
            }
        };
    }
}
