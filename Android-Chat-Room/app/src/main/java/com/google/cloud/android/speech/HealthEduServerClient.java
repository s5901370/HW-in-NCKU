package com.google.cloud.android.speech;

import com.google.gson.JsonObject;

import com.google.cloud.android.speech.HealthEduServerRequest;
import retrofit2.Call;
import retrofit2.http.Body;
import retrofit2.http.POST;
import retrofit2.http.Url;

public interface HealthEduServerClient {

    String BASE_URL = "http://140.116.247.161:8888/questionnaire/";

    // Request user_id from Health Education by line_id and line_email
    @POST("login_user_line/")
    Call<JsonObject> lineMappingLoginResponse(@Body HealthEduServerRequest.UserLoginServerRequest body);

    //request answerRecord_uuid (start this session) from Health Education by user_id and topic_id
    @POST("startQuestion/")
    Call<JsonObject> questionStartResponse(@Body HealthEduServerRequest.StartQuestionServerRequest body);

    //request question from Health Education by answerRecord_uuid
    @POST("new_selectionQuestion/")
    Call<JsonObject> questionSelectResponse(@Body HealthEduServerRequest.SelectQuestionServerRequest body);

    //post user answer to Health Education by answerRecord_uuid, state and userSelect
    // actually same as questionSelectResponse api
    @POST("new_selectionQuestion/")
    Call<JsonObject> answerCorrectResponse(@Body HealthEduServerRequest.AnswerCorrectServerRequest body);

    //get recommend video from Health Education by answerRecord_uuid
    @POST("getVideoId/")
    Call<JsonObject> getRecommendVideo(@Body HealthEduServerRequest.RecommendVideoServerRequest body);

    // get wronglist
    @POST("getAnswerResult/")
    Call<JsonObject> getAnswerResult(@Body HealthEduServerRequest.AnswerResultRequest body);
}
