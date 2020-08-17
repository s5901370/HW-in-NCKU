package com.google.cloud.android.speech;


import android.annotation.SuppressLint;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageButton;

import com.google.gson.JsonObject;
import android.graphics.Color;

import com.google.cloud.android.speech.R;
import com.google.cloud.android.speech.Global;
import com.google.cloud.android.speech.HealthEduServerRequest;
import com.google.cloud.android.speech.HealthEduServerClient;
import com.google.cloud.android.speech.SpeechFragment;
import com.google.cloud.android.speech.SpeechReaction;
import com.google.cloud.android.speech.DebugLogger;
import com.google.cloud.android.speech.TextUtil;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;


public class HEOptionFragment extends SpeechFragment {

    //UI
    private Button option1Btn, option2Btn;
    private ImageButton backBtn;

    // for health edu
    private String mQuestionText, mSubTypeText, mFeedbackText, mFeedbackSpeakText;
    private String mOptionA, mOptionB, mOptionC, mOptionD;
    private String mAnswer;
    private String mState;
    private Boolean mIsCorrect = false;



    public HEOptionFragment() {
    }


    public static HEOptionFragment newInstance() {
        HEOptionFragment fragment = new HEOptionFragment();
        Bundle args = new Bundle();
        fragment.setArguments(args);
        return fragment;
    }

    public static HEOptionFragment newInstance(Bundle args) {
        HEOptionFragment fragment = new HEOptionFragment();
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

                Log.d("HEOption", "Have C and D");
            }catch (NullPointerException e){
                Log.d("HEOption", "No C and D");
            }
        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View view = inflater.inflate(R.layout.fragment_heoption, container, false);

        initView(view);
        return view;
    }

    @SuppressLint("ResourceAsColor")
    private void initView(View view) {
        Global.api.robot.stopSpeak();
        Retrofit retrofit = new Retrofit.Builder().baseUrl(HealthEduServerClient.BASE_URL).addConverterFactory(GsonConverterFactory.create()).build();

        backBtn = view.findViewById(R.id.backButton);
        backBtn.setOnClickListener(v -> {
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
            }catch (NullPointerException e){
                Log.d("HEQuestion", "No C and D");
            }

            jumpNextFragment(HEQuestionFragment.newInstance(bundle));
        });

        option1Btn = view.findViewById(R.id.option1Button);
        option1Btn.setText(mOptionA);
        option1Btn.setOnClickListener(v -> {
            option2Btn.setEnabled(false);
            if ("A".equals(mAnswer)) {
                mIsCorrect = true;
                option1Btn.setBackgroundResource(R.color.correct);
                //option2Btn.setBackgroundResource(R.color.wrong);
            } else {
                mIsCorrect = false;
                option1Btn.setBackgroundResource(R.color.wrong);
                //option2Btn.setBackgroundResource(R.color.correct);
            }
            retrofit.create(HealthEduServerClient.class).answerCorrectResponse(
                    new HealthEduServerRequest.AnswerCorrectServerRequest("update", Global.answerRecord_uuid, "A")
            ).enqueue(new Callback<JsonObject>() {
                @Override
                public void onResponse(Call<JsonObject> call, Response<JsonObject> response) {
                    Log.d("HEOption", "AnswerCorrectServerRequest " + String.valueOf(response.body()));
                    mState = response.body().get("state").getAsString();
                    mFeedbackText = response.body().get("text").getAsString();
                    mFeedbackSpeakText = response.body().get("speak").getAsString();

                    Bundle bundle = new Bundle();
                    bundle.putString("state", mState);
                    bundle.putString("text", mFeedbackText);
                    bundle.putString("speak", mFeedbackSpeakText);
                    Handler handler = new Handler();
                    handler.postDelayed(new Runnable() {
                        @Override
                        public void run() {
                            Global.speakWithDiffFace(4000, mIsCorrect);
                            jumpNextFragment(HEFeedbackFragment.newInstance(bundle));
                        }
                    }, 500);

                }

                @Override
                public void onFailure(Call<JsonObject> call, Throwable t) {
                    Log.d("HEOption", "option1Btn AnswerCorrect onFailure " + String.valueOf(t));
                }
            });



        });

        option2Btn = view.findViewById(R.id.option2Button);
        option2Btn.setText(mOptionB);
        option2Btn.setOnClickListener(v -> {
            option1Btn.setEnabled(false);
            if ("B".equals(mAnswer)) {
                mIsCorrect = true;
                //option1Btn.setBackgroundResource(R.color.wrong);
                option2Btn.setBackgroundResource(R.color.correct);
            } else {
                mIsCorrect = false;
                //option1Btn.setBackgroundResource(R.color.correct);
                option2Btn.setBackgroundResource(R.color.wrong);
            }
            retrofit.create(HealthEduServerClient.class).answerCorrectResponse(
                    new HealthEduServerRequest.AnswerCorrectServerRequest("update", Global.answerRecord_uuid, "B")
            ).enqueue(new Callback<JsonObject>() {
                @Override
                public void onResponse(Call<JsonObject> call, Response<JsonObject> response) {
                    Log.d("HEOption", "AnswerCorrectServerRequest " + String.valueOf(response.body()));
                    mState = response.body().get("state").getAsString();
                    mFeedbackText = response.body().get("text").getAsString();
                    mFeedbackSpeakText = response.body().get("speak").getAsString();

                    Bundle bundle = new Bundle();
                    bundle.putString("state", mState);
                    bundle.putString("text", mFeedbackText);
                    bundle.putString("speak", mFeedbackSpeakText);
                    Handler handler = new Handler();
                    handler.postDelayed(new Runnable() {
                        @Override
                        public void run() {
                            Global.speakWithDiffFace(4000, mIsCorrect);
                            jumpNextFragment(HEFeedbackFragment.newInstance(bundle));
                        }
                    }, 500);

                }

                @Override
                public void onFailure(Call<JsonObject> call, Throwable t) {
                    Log.d("HEOption", "option1Btn AnswerCorrect onFailure " + String.valueOf(t));
                }
            });


        });

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
                String[] keywordsOfOptA = {"A", "上面", "第一個", "上", "選項一", mOptionA};
                String[] keywordsOfOptB = {"B", "下面", "第二個", "下", "選項二", mOptionB};

                if (TextUtil.inKeywords(text, keywordsOfStart) || TextUtil.inKeywords(text, keywordsOfRecommend)) {
                    jumpNextFragment(new MainActivityFragment());        // TODO: Need to define where to go
                }else if(TextUtil.inKeywords(text, keywordsOfOver)){
                    backBtn.performClick();
                }else if(TextUtil.inKeywords(text, keywordsOfOptA)){
                    option1Btn.performClick();
                }else if(TextUtil.inKeywords(text, keywordsOfOptB)){
                    option2Btn.performClick();
                }
            }
        };
    }
}
