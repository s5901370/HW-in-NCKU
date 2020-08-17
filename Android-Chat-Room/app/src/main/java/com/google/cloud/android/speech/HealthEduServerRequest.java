package com.google.cloud.android.speech;

import com.google.gson.JsonObject;
import com.google.gson.annotations.SerializedName;

public class HealthEduServerRequest {

    public static class UserLoginServerRequest{
        // POST
        @SerializedName("user_line_uuid")
        private String mLineId;

        // POST
        @SerializedName("user_email")
        private String mUserEmail;

        // return
        @SerializedName("state")
        private String mState;

        // return
        @SerializedName("user_id")
        private String mUserId;

        public UserLoginServerRequest(String mLineId, String mUserEmail){
            this.mLineId = mLineId;
            this.mUserEmail = mUserEmail;
        }

        public void setUserIdAndState(String mState, String mUserId){
            this.mState = mState;
            this.mUserId = mUserId;
        }

        public String getState(){
            return this.mState;
        }

        public String getUserId(){
            return this.mUserId;
        }

    }

    public static class StartQuestionServerRequest{

        // POST
        @SerializedName("topic_id")
        private String mTopicId;

        // POST
        @SerializedName("user_id")
        private String mUserId;

        // return
        @SerializedName("state")
        private String mState;

        // return
        @SerializedName("questionNum")
        private String mQuestionNum;

        // return
        @SerializedName("answerRecord_uuid")
        private String mAnswerRecordId;

        public StartQuestionServerRequest(String mTopicId, String mUserId){
            this.mTopicId = mTopicId;
            this.mUserId = mUserId;
        }

        public void setStartQuestion(String mState, String mQuestionNum, String mAnswerRecordId){
            this.mState = mState;
            this.mQuestionNum = mQuestionNum;
            this.mAnswerRecordId = mAnswerRecordId;
        }

        public String getState(){
            return this.mState;
        }

        public String getQuestionNum(){
            return this.mQuestionNum;
        }

        public String getAnswerRecordId(){
            return this.mAnswerRecordId;
        }

    }

    public static class SelectQuestionServerRequest{
        // POST
        @SerializedName("state")
        private String mState;

        // POST
        @SerializedName("answerRecord_uuid")
        private String mAnswerRecordId;

        // return
//        @SerializedName("state")
//        private String mReturnState;

        // return
        @SerializedName("question")
        private String mQuestion;

        // return
        @SerializedName("options")
        private JsonObject mOptions;

        // return
        @SerializedName("answer")
        private String mAnswer;

        public SelectQuestionServerRequest(String mState, String mAnswerRecordId){
            this.mState = mState;
            this.mAnswerRecordId = mAnswerRecordId;
        }

        public void setSelectQuestion(String mState, String mQuestion, JsonObject mOptions, String mAnswer){
            this.mState = mState;
            this.mQuestion = mQuestion;
            this.mOptions = mOptions;
            this.mAnswer = mAnswer;
        }

        public String getReturnState(){
            return this.mState;
        }

        public String getQuestion(){
            return this.mQuestion;
        }

        public JsonObject getHealthOptions(){
            return this.mOptions;
        }

        public String getAnswer(){
            return this.mAnswer;
        }
    }


    public static class AnswerCorrectServerRequest{
        // POST
        @SerializedName("state")
        private String mState;

        // POST
        @SerializedName("answerRecord_uuid")
        private String mAnswerRecordId;

        // POST
        @SerializedName("userSelected")
        private String mUserSelected;

        // return
        @SerializedName("user_ability")
        private String mUserAbility;

        // return
        @SerializedName("correct")
        private int mCorrect;

        // return
        @SerializedName("text")
        private String mText;

        // return
        @SerializedName("speak")
        private String mSpeak;


        public AnswerCorrectServerRequest(String mState, String mAnswerRecordId, String mUserSelected){
            this.mState = mState;
            this.mAnswerRecordId = mAnswerRecordId;
            this.mUserSelected = mUserSelected;
        }

        public void setAnswerCorrect(String mReturnState, String mUserAbility, int mCorrect, String mText, String mSpeak){
            this.mState = mState;
            this.mUserAbility = mUserAbility;
            this.mCorrect = mCorrect;
            this.mText = mText;
            this.mSpeak = mSpeak;
        }

        public String getReturnState(){
            return this.mState;
        }

        public String getUserAbility(){
            return this.mUserAbility;
        }

        public int getCorrect(){
            return this.mCorrect;
        }

        public String getReturnText(){
            return this.mText;
        }

        public String getRetrunSpeak(){
            return this.mSpeak;
        }

    }

    public static class RecommendVideoServerRequest{
        // POST
        @SerializedName("answerRecord_uuid")
        private String mAnswerRecordId;

        // return
        @SerializedName("video_id")
        private String mVideoId;

        public RecommendVideoServerRequest(String mAnswerRecordId){
            this.mAnswerRecordId = mAnswerRecordId;
        }

        public void setVideoId(String mVideoId){
            this.mVideoId = mVideoId;
        }

        public String getVideoId(){
            return this.mVideoId;
        }

    }

    public static class AnswerResultRequest{
        // POST
        @SerializedName("answerRecord_uuid")
        private String mAnswerRecordId;

        // return
        @SerializedName("answerRecord")
        private JsonObject mAnswerRecord;
//         "answerRecord": {'2': 0,...},
//                "correct_num": 3,
//                "false_num": 2,
//                "wrong_feedback": {{q_id:"test"},...}

        public AnswerResultRequest(String mAnswerRecordId){
            this.mAnswerRecordId = mAnswerRecordId;
        }

        public void setAnswerRecord(JsonObject mAnswerRecord){
            this.mAnswerRecord = mAnswerRecord;
        }

        public JsonObject getAnswerRecord(){
            return this.mAnswerRecord;
        }

    }

}
