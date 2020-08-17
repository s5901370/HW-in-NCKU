package com.google.cloud.android.speech;

import android.os.Bundle;
import android.support.constraint.ConstraintLayout;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.TextView;

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

import static com.google.cloud.android.speech.Global.wrongList;

public class HEFeedbackFragment extends SpeechFragment {

    // UI
    private ImageButton mHomeBtn;
    private TextView mFeedbackTextView;
    private Button mNextBtn;
    private ConstraintLayout mConstraintLayout;

    // for health edu
    // from pre question
    private String mState, mFeedbackText, mFeedbackSpeakText, mVideoId;
    // for another question
    private String mQuestionText, mSubTypeText;
    private JsonObject mOptions;
    private String mAnswer;



    public HEFeedbackFragment() {
        // Required empty public constructor
    }


    public static HEFeedbackFragment newInstance() {
        HEFeedbackFragment fragment = new HEFeedbackFragment();
        Bundle args = new Bundle();
        fragment.setArguments(args);
        return fragment;
    }

    public static HEFeedbackFragment newInstance(Bundle args) {
        HEFeedbackFragment fragment = new HEFeedbackFragment();
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (getArguments() != null) {
            mState = getArguments().getString("state");
            mFeedbackText = getArguments().getString("text");
            mFeedbackSpeakText = getArguments().getString("speak");
        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View view = inflater.inflate(R.layout.fragment_hefeedback, container, false);

        initView(view);
        return view;

    }

    private void initView(View view) {
        Global.api.robot.speak(mFeedbackSpeakText);
        Retrofit retrofit = new Retrofit.Builder().baseUrl(HealthEduServerClient.BASE_URL).addConverterFactory(GsonConverterFactory.create()).build();

        mHomeBtn = view.findViewById(R.id.homeButton);
        mHomeBtn.setOnClickListener(v -> {
            jumpNextFragment(new MainActivityFragment());
        });

        mFeedbackTextView = view.findViewById(R.id.feedbackText);
        mFeedbackTextView.setText(mFeedbackText);

        mConstraintLayout = view.findViewById(R.id.rootConstraintLayout);
        mConstraintLayout.setOnClickListener(v -> {
            // get next question from health edu server
            retrofit.create(HealthEduServerClient.class).questionSelectResponse(
                    new HealthEduServerRequest.SelectQuestionServerRequest("select", Global.answerRecord_uuid)
            ).enqueue(new Callback<JsonObject>() {
                          @Override
                          public void onResponse(Call<JsonObject> call, Response<JsonObject> response) {
                              Log.d("Feedback", "SelectQuestionServerRequest " + String.valueOf(response.body()));
                              if(!response.body().get("state").getAsString().equals("isEnd")){
                                  // contiune
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
                              else{
                                  // end
                                  // get video
                                  retrofit.create(HealthEduServerClient.class).getRecommendVideo(
                                          new HealthEduServerRequest.RecommendVideoServerRequest(Global.answerRecord_uuid)
                                  ).enqueue(new Callback<JsonObject>() {
                                      @Override
                                      public void onResponse(Call<JsonObject> call, Response<JsonObject> response) {
                                          Log.d("Feedback", "RecommendVideoServerRequest " + String.valueOf(response.body()));
                                          Log.d("Feedback", " " + Global.answerRecord_uuid);
                                          mVideoId = response.body().get("video_id").getAsString();
                                          Bundle bundle = new Bundle();
                                          bundle.putString("video_id", mVideoId);

                                          jumpNextFragment(HEVideoFragment.newInstance(bundle));
                                      }

                                      @Override
                                      public void onFailure(Call<JsonObject> call, Throwable t) {
                                          Log.d("Feedback", "initView RecommendVideoServerRequest onFailure " + String.valueOf(t));
                                      }
                                  });
                              }

                          }

                          @Override
                          public void onFailure(Call<JsonObject> call, Throwable t) {
                              Log.d("Feedback", "initView SelectQuestion onFailure " + String.valueOf(t));
                          }
                      }
            );
        });

        mNextBtn = view.findViewById(R.id.nextButton);
        mNextBtn.setOnClickListener(v -> {
            // get next question from health edu server
            retrofit.create(HealthEduServerClient.class).questionSelectResponse(
                    new HealthEduServerRequest.SelectQuestionServerRequest("select", Global.answerRecord_uuid)
            ).enqueue(new Callback<JsonObject>() {
                          @Override
                          public void onResponse(Call<JsonObject> call, Response<JsonObject> response) {
                              Log.d("Feedback", "SelectQuestionServerRequest " + String.valueOf(response.body()));
                              if(!response.body().get("state").getAsString().equals("isEnd")){
                                  // contiune
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
                              else{
                                  // end
                                  // get video
                                  retrofit.create(HealthEduServerClient.class).getRecommendVideo(
                                          new HealthEduServerRequest.RecommendVideoServerRequest(Global.answerRecord_uuid)
                                  ).enqueue(new Callback<JsonObject>() {
                                      @Override
                                      public void onResponse(Call<JsonObject> call, Response<JsonObject> response) {
                                          Log.d("Feedback", "RecommendVideoServerRequest " + String.valueOf(response.body()));
                                          Log.d("Feedback", " " + Global.answerRecord_uuid);
                                          mVideoId = response.body().get("video_id").getAsString();
                                          Bundle bundle = new Bundle();
                                          bundle.putString("video_id", mVideoId);

                                          jumpNextFragment(HEVideoFragment.newInstance(bundle));
                                      }

                                      @Override
                                      public void onFailure(Call<JsonObject> call, Throwable t) {
                                          Log.d("Feedback", "initView RecommendVideoServerRequest onFailure " + String.valueOf(t));
                                      }
                                  });
                              }

                          }

                          @Override
                          public void onFailure(Call<JsonObject> call, Throwable t) {
                              Log.d("Feedback", "initView SelectQuestion onFailure " + String.valueOf(t));
                          }
                      }
            );
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
                String[] keywordsOfNext = {"下面","下一題" , "下一頁", "選項", "看", "next", "page"};

                if (TextUtil.inKeywords(text, keywordsOfStart) || TextUtil.inKeywords(text, keywordsOfRecommend)) {
                    jumpNextFragment(new MainActivityFragment());
                }else if(TextUtil.inKeywords(text, keywordsOfOver)){
                    mHomeBtn.performClick();
                }else if(TextUtil.inKeywords(text, keywordsOfNext)){
                    mNextBtn.performClick();
                }
            }
        };
    }
}
