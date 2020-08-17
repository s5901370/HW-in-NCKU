package com.google.cloud.android.speech;

public class MyMessage {
    private String text; // message body
    private int belongsToCurrentUser; // is this message sent by us?
    private String time;
    public MyMessage(String text,  int belongsToCurrentUser,String time) {
        this.text = text;
        this.belongsToCurrentUser = belongsToCurrentUser;
        this.time = time;
    }
    public String getTime(){
        return time;
    }
    public String getText() {
        return text;
    }


    public int isBelongsToCurrentUser() {
        return belongsToCurrentUser;
    }
}
