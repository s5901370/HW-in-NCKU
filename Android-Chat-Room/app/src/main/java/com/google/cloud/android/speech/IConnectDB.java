package com.google.cloud.android.speech;

import java.io.UnsupportedEncodingException;
import java.util.HashMap;

public interface IConnectDB {
    public Connect_db getConnectDB();

    /* Database */
    void onGetMessage_ques(HashMap<String, String[]> results);

    void onGetMessage_ques2(HashMap<String, String[]> results);

    void onGetMessage_items(HashMap<String, String[]> results) throws UnsupportedEncodingException;
}
