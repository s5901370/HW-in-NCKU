package com.google.cloud.android.speech;


import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

import com.google.android.youtube.player.YouTubeInitializationResult;
import com.google.android.youtube.player.YouTubePlayer;
import com.google.android.youtube.player.YouTubePlayerFragment;
import com.google.android.youtube.player.YouTubePlayerSupportFragment;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;

import java.util.HashMap;
import java.util.Map;
import java.util.Set;

import com.google.cloud.android.speech.R;
import com.google.cloud.android.speech.Global;
import com.google.cloud.android.speech.PlayerConfig;
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
import static com.google.cloud.android.speech.R.id.playBtn;
import static com.google.cloud.android.speech.R.id.youtube_fragment;


public class HEVideoFragment extends SpeechFragment {
    // UI
    private Button mExitBtn, mPlayBtn;
    private TextView mCurrentSrc;

    // Youtube
    YouTubePlayerSupportFragment mYouTubePlayerFragment;
    private String mVideoId;
    private String sourceUrl;
    private YouTubePlayer mYouTubePlayer;
    private boolean isPlay = true;

    // HE
    private JsonObject mRecord;

    public HEVideoFragment() {
        // Required empty public constructor
    }

    public static HEVideoFragment newInstance() {
        HEVideoFragment fragment = new HEVideoFragment();
        Bundle args = new Bundle();
        fragment.setArguments(args);
        return fragment;
    }

    public static HEVideoFragment newInstance(Bundle args) {
        HEVideoFragment fragment = new HEVideoFragment();
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (getArguments() != null) {
            mVideoId = getArguments().getString("video_id");
        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View view = inflater.inflate(R.layout.fragment_hevideo, container, false);
        initView(view);
        Log.d("Video", "onCreateView");
        return view;
    }

    private void initView(View view){
        mYouTubePlayerFragment = (YouTubePlayerSupportFragment)getChildFragmentManager().findFragmentById(youtube_fragment);
        initYoutube();
        sourceUrl = "來源:https://www.youtube.com/watch?v=" + mVideoId;
        mCurrentSrc = view.findViewById(R.id.currentSrc);
        mCurrentSrc.setText(sourceUrl);

        mExitBtn = view.findViewById(R.id.backBtn);
        mExitBtn.setOnClickListener(v -> {
            jumpNextFragment(new URLFragment());
        });
        mPlayBtn = view.findViewById(R.id.playBtn);
        mPlayBtn.setOnClickListener(v -> {
            if(isPlay){
                mYouTubePlayer.play();
                mPlayBtn.setText("暫停");
                isPlay = false;
            }
            else {
                mYouTubePlayer.pause();
                mPlayBtn.setText("播放");
                isPlay = true;
            }

        });

        // get wrong list
        Retrofit retrofit = new Retrofit.Builder().baseUrl(HealthEduServerClient.BASE_URL).addConverterFactory(GsonConverterFactory.create()).build();
        retrofit.create(HealthEduServerClient.class).getAnswerResult(
                new HealthEduServerRequest.AnswerResultRequest(Global.answerRecord_uuid)
        ).enqueue(new Callback<JsonObject>() {
            @Override
            public void onResponse(Call<JsonObject> call, Response<JsonObject> response) {
                Log.d("Feedback", "AnswerResultRequest " + String.valueOf(response.body()));

                mRecord = response.body().get("answerRecord").getAsJsonObject();

                Map<String, Object> attributes = new HashMap<String, Object>();
                Set<Map.Entry<String, JsonElement>> entrySet = mRecord.entrySet();
                wrongList.clear();
                for(Map.Entry<String,JsonElement> entry : entrySet){
                    if (mRecord.get(entry.getKey()).getAsInt()==0)//0 is wrong
                        wrongList.add(entry.getKey());
                }
                Log.i("wronglist",wrongList.toString());
            }

            @Override
            public void onFailure(Call<JsonObject> call, Throwable t) {
                Log.d("Feedback", "initView AnswerResultRequest onFailure " + String.valueOf(t));
            }
        });

    }

    private void initYoutube(){
        //invoke youtube app to play the video
        //with YoutubeAPI
        mYouTubePlayerFragment.initialize(PlayerConfig.API_KEY, new YouTubePlayer.OnInitializedListener() {

            @Override
            public void onInitializationSuccess(YouTubePlayer.Provider provider, YouTubePlayer youTubePlayer, boolean b) {

                youTubePlayer.cueVideo(mVideoId);
                mYouTubePlayer = youTubePlayer;

            }

            @Override
            public void onInitializationFailure(YouTubePlayer.Provider provider, YouTubeInitializationResult youTubeInitializationResult) {

            }
        });
        Log.d("Video", "Video Playing....");
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
                String[] keywordsOfVideo = {"播放","看", "影片", "play", "pause", "暫停", "播", "停止"};

                if (TextUtil.inKeywords(text, keywordsOfStart) || TextUtil.inKeywords(text, keywordsOfRecommend)) {
                    jumpNextFragment(new URLFragment());
                }else if(TextUtil.inKeywords(text, keywordsOfOver)){
                    mExitBtn.performClick();
                }else if(TextUtil.inKeywords(text, keywordsOfNext)){
                    mExitBtn.performClick();
                }else if(TextUtil.inKeywords(text, keywordsOfVideo)){
                    mPlayBtn.performClick();
                }
            }
        };
    }
}
