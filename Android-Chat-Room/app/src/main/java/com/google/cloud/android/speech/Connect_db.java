package com.google.cloud.android.speech;

import android.content.Context;
import android.util.Log;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.UnsupportedEncodingException;
import java.util.HashMap;

/**
 * Created by hyliu on 2018/5/30.
 */

public class Connect_db {

    public static HashMap<String, String[]> results = new HashMap<String, String[]>();
    private static RequestQueue requestQueue;
    private IConnectDB db;
    private String[] question;
    private String[] ans_1;
    private String[] ans_2;
    private String[] type;
    private String[] img_url;
    private String[] title;
    private String[] brief_info;
    private String[] detailed_info;

    //    private int [] get_type  = {4,2,3};
//    private int [] get_ans  = {0,0,0};
    private String[] tag_1, tag_2, tag_3;
    private String[] ratings, links;
    private int data_len = 3;
    private String TAG = "Connect_db";

    public Connect_db(IConnectDB db) {
        this.db = db;
    }

    public void get_question(String domain, Context context) {
        //914 change db
        //String showUrl = "http://140.116.247.169/get_question.php?domain=" + domain;
        String showUrl = "http://140.116.247.183/zenbo/get_question.php?domain=" + domain;
        Log.d(TAG, "1234");
        requestQueue = Volley.newRequestQueue(context);

        JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(Request.Method.GET, showUrl, null, new Response.Listener<JSONObject>() {
            @Override
            public void onResponse(JSONObject response) {
                //Log.d(TAG, response.toString());

                try {
                    JSONArray data = response.getJSONArray("data");
                    data_len = data.length();
                    question = new String[data_len];
                    ans_1 = new String[data_len];
                    ans_2 = new String[data_len];
                    type = new String[data_len];

                    for (int i = 0; i < data.length(); i++) {

                        JSONObject jsondata = data.getJSONObject(i);

                        question[i] = jsondata.getString("question");
                        ans_1[i] = jsondata.getString("ans_1");
                        ans_2[i] = jsondata.getString("ans_2");
                        type[i] = jsondata.getString("type");
                    }
                    results.put("question", question);
                    results.put("ans_1", ans_1);
                    results.put("ans_2", ans_2);
                    results.put("type", type);
                    db.onGetMessage_ques(results);
                } catch (JSONException e) {
                    e.printStackTrace();
                }
            }
        }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {

                Log.d(TAG, error.getMessage());
            }
        });
        requestQueue.add(jsonObjectRequest);
    }

    public void get_question2(String domain, Context context, int next_type) {
        //914 change db
        //String showUrl = "http://140.116.247.169/get_question.php?domain=" + domain;
        String showUrl = "http://140.116.247.183/zenbo/get_question.php?domain=" + domain;

        if (next_type > 0) {
            showUrl = showUrl + "&qs_type=" + next_type;
        }

        requestQueue = Volley.newRequestQueue(context);

        JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(Request.Method.GET, showUrl, null, new Response.Listener<JSONObject>() {
            @Override
            public void onResponse(JSONObject response) {
                //Log.d(TAG, response.toString());

                try {
                    JSONArray data = response.getJSONArray("data");
                    data_len = data.length();
                    question = new String[data_len];
                    ans_1 = new String[data_len];
                    ans_2 = new String[data_len];
                    type = new String[data_len];
                    String[] act_type1 = new String[data_len];
                    String[] act_type2 = new String[data_len];

                    for (int i = 0; i < data.length(); i++) {

                        JSONObject jsondata = data.getJSONObject(i);

                        question[i] = jsondata.getString("question");
                        ans_1[i] = jsondata.getString("ans_1");
                        ans_2[i] = jsondata.getString("ans_2");
                        type[i] = jsondata.getString("type");
                        act_type1[i] = jsondata.getString("act_type1");
                        act_type2[i] = jsondata.getString("act_type2");

                    }

                    results.put("question", question);
                    results.put("ans_1", ans_1);
                    results.put("ans_2", ans_2);
                    results.put("type", type);
                    results.put("act_type1", act_type1);
                    results.put("act_type2", act_type2);
                    db.onGetMessage_ques2(results);
                } catch (JSONException e) {
                    e.printStackTrace();
                }
            }
        }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {

                Log.d(TAG, error.getMessage());
            }
        });
        requestQueue.add(jsonObjectRequest);
    }

    public void get_items(String domain, Context context, int[] get_ans, String[] get_type, String poi_response) {


        //914 change db

        //String showUrl = "http://140.116.247.169/get_items.php?";
        //String showUrl = "http://140.116.247.183/zenbo/get_items.php?";
        String showUrl = "http://140.116.247.183/zenbo/get_items.php?";
        if (domain.equals("news")) {
            //914 change db
            //showUrl = "http://140.116.247.169/get_items.php?domain=" + domain + "&type1=" + get_type[0] + "&type2=" + get_type[1]
            //        + "&type3=" + get_type[2] + "&ans1=" + get_ans[0] + "&ans2=" + get_ans[1] + "&ans3=" + get_ans[2];
            showUrl = "http://140.116.247.183/zenbo/get_items.php?domain=" + domain + "&type1=" + get_type[0] + "&type2=" + get_type[1]
                    + "&type3=" + get_type[2] + "&ans1=" + get_ans[0] + "&ans2=" + get_ans[1] + "&ans3=" + get_ans[2];
        } else if (domain.equals("activity")) {  //改poi資料庫
            //914 change db
            //showUrl = "http://140.116.247.169/get_items.php?domain=" + domain + "&poi_response=" + poi_response;
            //showUrl = "http://140.116.247.183/zenbo/get_items.php?domain=" + domain + "&poi_response=" + poi_response;
            showUrl = "http://140.116.247.183/zenbo/get_items.php?domain=" + domain + "&poi_response=" + poi_response;
        }

        Log.d("show Url Successful", showUrl);

        requestQueue = Volley.newRequestQueue(context);

        //JSONObject jobj = new JSONObject();

        JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(Request.Method.GET, showUrl, null, new Response.Listener<JSONObject>() {
            @Override
            public void onResponse(JSONObject response) {
                try {
                    JSONArray data = response.getJSONArray("data");
                    data_len = data.length();
                    img_url = new String[data_len];
                    title = new String[data_len];
                    brief_info = new String[data_len];
                    detailed_info = new String[data_len];
                    tag_1 = new String[data_len];
                    tag_2 = new String[data_len];
                    tag_3 = new String[data_len];
                    ratings = new String[data_len];
                    links = new String[data_len];

                    for (int i = 0; i < data.length(); i++) {

                        JSONObject jsondata = data.getJSONObject(i);

                        img_url[i] = jsondata.getString("image_url");
                        title[i] = jsondata.getString("title");
                        brief_info[i] = jsondata.getString("brief_info");
                        detailed_info[i] = jsondata.getString("detailed_info");
                        tag_1[i] = jsondata.getString("tag_1");
                        tag_2[i] = jsondata.getString("tag_2");
                        tag_3[i] = jsondata.getString("tag_3");
                        ratings[i] = jsondata.getString("rating");
                        links[i] = jsondata.getString("link");
                        //Log.d(TAG, que + a1);
                    }

                    results.put("img_url", img_url);
                    results.put("title", title);
                    results.put("brief_info", brief_info);
                    results.put("detailed_info", detailed_info);
                    results.put("tag_1", tag_1);
                    results.put("tag_2", tag_2);
                    results.put("tag_3", tag_3);
                    results.put("rating", ratings);
                    results.put("link", links);

                    db.onGetMessage_items(results);

                } catch (JSONException e) {
                    e.printStackTrace();
                } catch (UnsupportedEncodingException e) {
                    e.printStackTrace();
                }
            }
        }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {

                Log.d(TAG, error.getMessage());
            }
        });
        requestQueue.add(jsonObjectRequest);
    }
}
