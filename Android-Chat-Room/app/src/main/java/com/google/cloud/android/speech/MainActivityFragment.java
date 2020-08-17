package com.google.cloud.android.speech;


import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageButton;

import com.google.gson.JsonObject;

import com.google.cloud.android.speech.R;
import com.google.cloud.android.speech.Global;
import com.google.cloud.android.speech.HealthEduServerRequest;
import com.google.cloud.android.speech.HealthEduServerClient;
import com.google.cloud.android.speech.SpeechFragment;
import com.google.cloud.android.speech.SpeechReaction;

import com.google.cloud.android.speech.TextUtil;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;



/**
 Start page, menu for all function
 */
public class MainActivityFragment extends SpeechFragment {

    //UI
    private ImageButton mWeatherBtn, mRecommendBtn, mHealthBtn, mHealthRecBtn;

    //for health edu
    private String mQuestionText, mSubTypeText;
    private JsonObject mOptions;
    private String mAnswer;
    private String mState;


    public MainActivityFragment() {
        // Required empty public constructor
    }

    public static MainActivityFragment newInstance(String param1, String param2) {
        MainActivityFragment fragment = new MainActivityFragment();
        Bundle args = new Bundle();

        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (getArguments() != null) {

        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View view = inflater.inflate(R.layout.fragment_main_menu, container, false);
        initView(view);
        Log.d("main", "onCreateView");
        return view;
    }

    private void initView(View view){

        Log.d("main", "initView");

//        // defines weather button behavior
//        mWeatherBtn = view.findViewById(R.id.weatherButton);
//        mWeatherBtn.setOnClickListener(v ->{
//            jumpNextFragment(new WeatherFragment());
//        });
//
//
//        mHealthRecBtn = view.findViewById(R.id.healthRecButton);
//        mHealthRecBtn.setOnClickListener(v ->{
//            Log.d("main", "HealthRec");
//            jumpNextFragment(HealthMenuFragment.newInstance());
//        });
//
//
//        mRecommendBtn = view.findViewById(R.id.recommendButton);
//        mRecommendBtn.setOnClickListener(v ->{
//            Log.d("main", "Rec");
//            jumpNextFragment(new RecMenuFragment());
//        });

        mHealthBtn = view.findViewById(R.id.healthButton);
        mHealthBtn.setOnClickListener(v ->{
            Log.d("main", "hedu");
            // get user id from health edu server
            Retrofit retrofit = new Retrofit.Builder().baseUrl(HealthEduServerClient.BASE_URL).addConverterFactory(GsonConverterFactory.create()).build();
            retrofit.create(HealthEduServerClient.class).lineMappingLoginResponse(
                    new HealthEduServerRequest.UserLoginServerRequest(Global.LineUserUUID, Global.currentUserEmail)
            ).enqueue(new Callback<JsonObject>() {
                @Override
                public void onResponse(Call<JsonObject> call, Response<JsonObject> response) {
                    Log.d("main", "UserLoginServerRequest " + String.valueOf(response.body()));
                    Global.HEuserId = String.valueOf(response.body().getAsJsonObject().get("user_id"));
                    Log.d("main", "UserId " + Global.HEuserId);

                    // get question id from health edu server
                    retrofit.create(HealthEduServerClient.class).questionStartResponse(
                            new HealthEduServerRequest.StartQuestionServerRequest(String.valueOf(Global.HealthTopicId), Global.HEuserId)
                    ).enqueue(new Callback<JsonObject>() {
                        @Override
                        public void onResponse(Call<JsonObject> call, Response<JsonObject> response) {
                            Log.d("main", "StartQuestionServerRequest " + String.valueOf(response.body()));
                            Global.questionNum = response.body().getAsJsonObject().get("questionNum").getAsInt();
                            Global.answerRecord_uuid = response.body().getAsJsonObject().get("answerRecord_uuid").getAsString();
                            Log.d("main", Global.answerRecord_uuid);
                            // get question from health edu server
                            retrofit.create(HealthEduServerClient.class).questionSelectResponse(
                                    new HealthEduServerRequest.SelectQuestionServerRequest("select", Global.answerRecord_uuid)
                            ).enqueue(new Callback<JsonObject>() {
                                          @Override
                                          public void onResponse(Call<JsonObject> call, Response<JsonObject> response) {
                                              Log.d("main", "SelectQuestionServerRequest " + String.valueOf(response.body()));

                                              mQuestionText = response.body().get("question").getAsString();
                                              mSubTypeText = response.body().get("question_type").getAsString();
                                              mOptions = response.body().get("options").getAsJsonObject();
                                              mAnswer = response.body().get("answer").getAsString();
                                              mState = response.body().get("state").getAsString();

                                              Bundle bundle = new Bundle();
                                              bundle.putString("questionText", mQuestionText);
                                              bundle.putString("subtypeText", mSubTypeText);
                                              bundle.putString("answer", mAnswer);
                                              bundle.putString("state", mState);
                                              bundle.putString("A", mOptions.get("A").getAsString());
                                              bundle.putString("B", mOptions.get("B").getAsString());
                                              if(!mOptions.get("C").getAsString().equals("")){
                                                  bundle.putString("C", mOptions.get("C").getAsString());
                                              }
                                              if(!mOptions.get("D").getAsString().equals("")){
                                                  bundle.putString("D", mOptions.get("D").getAsString());
                                              }

                                              jumpNextFragment(HEQuestionFragment.newInstance(bundle));

                                          }

                                          @Override
                                          public void onFailure(Call<JsonObject> call, Throwable t) {
                                              Log.d("main", "initView SelectQuestion onFailure " + String.valueOf(t));
                                          }
                                      }
                            );

                        }

                        @Override
                        public void onFailure(Call<JsonObject> call, Throwable t) {
                            Log.d("main", "mHealthBtn startQuesiton onFailure " + String.valueOf(t));
                        }
                    });

                }

                @Override
                public void onFailure(Call<JsonObject> call, Throwable t) {
                    Log.d("main", "mHealthBtn login onFailure " + String.valueOf(t));
                }
            });

        });

    }



    @Override
    public SpeechReaction getSpeechReaction() {
        Log.d("MainActivityFragment","getSpeechReaction");
        return new SpeechReaction() {
            @Override
            public void onSentenceEnd(String text) {
                Log.d("MainActivityFragment","SentenceEnd");

                String[] keywordsOfHealthEdu = {"衛教", "味覺", "衛生", "教育", "題目", "題庫"};
                if(TextUtil.inKeywords(text, keywordsOfHealthEdu)){
                    mHealthBtn.performClick();
                }
            }
        };
    }

}
