package com.example.zerone.moviehelper;


import android.net.Uri;
import android.util.Log;
import android.widget.Toast;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.Reader;
import java.net.HttpURLConnection;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;
import java.net.URL;
import java.net.URLEncoder;


public class Network {

    private static final String Server_IP="http://192.168.0.102:8000/search_movie/";


    //movie 為輸入的電影
    static String getTheatre(String movie){
        //Socket socketConnection = null;
        HttpURLConnection urlConnection = null;
        BufferedReader reader = null;
        String Theatres = null;
        StringBuilder response = new StringBuilder();

        try{
//            Uri builtURI = Uri.parse(Server_IP).buildUpon().build();
            //SocketAddress sockAddr = new InetSocketAddress(Server_IP,8000);

            //socketConnection = new Socket();
            //socketConnection.connect(sockAddr,2500);
            URL url = new URL(Server_IP);
            urlConnection = (HttpURLConnection) url.openConnection();
            urlConnection.setRequestProperty("Content-Type", "application/json;charset=UTF-8");
            urlConnection.setRequestProperty("Accept","application/json");
//            urlConnection.setRequestProperty( "Accept", "application/json");
            urlConnection.setRequestMethod("POST");
            urlConnection.setDoOutput(true);
            urlConnection.setDoInput(true);
            //urlConnection.connect();

            JSONObject jsonParam =new JSONObject();
            try {
                jsonParam.put("movie", movie);
            }catch (JSONException e){
                e.printStackTrace();
            }

            Log.i("JSON", jsonParam.toString());
            DataOutputStream os = new DataOutputStream(urlConnection.getOutputStream());
//            os.writeBytes(URLEncoder.encode(jsonParam.toString(), "UTF-8"));
//            os.writeBytes(jsonParam.toString());
            String s = jsonParam.toString();
            os.write(s.getBytes());

            os.flush();
            os.close();

            //Get Response
            InputStream is = urlConnection.getInputStream();

            reader = new BufferedReader(new InputStreamReader(is));
            String line;

            while ((line = reader.readLine()) != null) {
                response.append(line);
                response.append('\r');
            }
            reader.close();
            Theatres = response.toString();
            Log.i("MSG" , response.toString());

//            Log.i("STATUS", String.valueOf(urlConnection.getResponseCode()));
//            Log.i("MSG" , urlConnection.getResponseMessage());
//            urlConnection.disconnect();
//            try {
//                jsonParam.put("movie", "MIB星際戰警：跨國行動");
//            }catch (JSONException e){
//                e.printStackTrace();
//            }
//            DataOutputStream os =new DataOutputStream(urlConnection.getOutputStream());
//            os.writeBytes(jsonParam.toString());
//
//            os.flush();
//            os.close();
//
//            Log.i("STATUS",String.valueOf(urlConnection.getResponseCode()));
//            Log.i("MSG" , urlConnection.getResponseMessage());
            // Get the InputStream.
            //InputStream inputStream = urlConnection.getInputStream();

            // Create a buffered reader from that input stream.
            /*reader = new BufferedReader(new InputStreamReader(inputStream));

            // Use a StringBuilder to hold the incoming response.
            StringBuilder builder = new StringBuilder();

            String line;
            while ((line = reader.readLine()) != null) {
                builder.append(line);
                // Since it's JSON, adding a newline isn't necessary (it won't
                // affect parsing) but it does make debugging a *lot* easier
                // if you print out the completed buffer for debugging.
                builder.append("\n");

            }
            if (builder.length() == 0) {
                // Stream was empty. No point in parsing.
                return "success";
            }*/
            //Theatres = builder.toString();

        }catch (IOException e) {
            e.printStackTrace();
        } finally {
            if ( urlConnection!= null) {
                urlConnection.disconnect();
            }
            if (reader != null) {
                try {
                    reader.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }

        return Theatres;
    }
}
