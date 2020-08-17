package com.google.cloud.android.speech;

import android.content.Context;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.HashMap;

import com.google.cloud.android.speech.R;

public class TextUtil {
    public static String[] cityNamesSimple = {"基隆", "台北", "新北", "桃園", "新竹", "苗栗", "台中", "彰化", "雲林","嘉義",
            "台南", "高雄", "屏東", "宜蘭", "花蓮", "台東", "南投", "澎湖", "馬祖", "媽祖", "連江", "金門", "綠島", "蘭嶼",
            "龜山島", "小琉球"};

    public static String[] cityNamesComplete = {"雲林縣", "南投縣", "連江縣", "臺東縣", "台東縣", "金門縣", "宜蘭縣", "屏東縣",
            "苗栗縣", "澎湖縣", "臺北市", "台北市", "新竹縣", "花蓮縣", "高雄市", "彰化縣", "新竹市", "新北市", "基隆市",
            "臺中市", "台中市", "臺南市", "台南市", "桃園市", "嘉義縣", "嘉義市"};

    public static String mappingOfCities(String city){
        // e.g. 台南-> 台南市, 台南縣-> 台南市, 台南市-> 台南市
        switch (city){
            case "台北":
                return "台北市";
            case "台北縣":
            case "新北":
            case "新北縣":
                return "新北市";
            case "基隆":
            case "基隆縣":
                return "基隆市";
            case "桃園":
            case "桃園縣":
                return "桃園市";
            case "新竹":
                return "新竹市";
            case "苗栗":
            case "苗栗市":
                return "苗栗縣";
            case "台中":
            case "台中縣":
                return "台中市";
            case "彰化":
            case "彰化市":
                return "彰化縣";
            case "雲林市":
            case "雲林":
                return "雲林縣";
            case "南投":
            case "南投市":
                return "南投縣";
            case "嘉義":
                return "嘉義市";
            case "台南":
            case "台南縣":
                return "台南市";
            case "高雄":
            case "高雄縣":
                return "高雄市";
            case "屏東":
            case "屏東市":
            case "小琉球":
                return "屏東縣";
            case "宜蘭":
            case "宜蘭市":
            case "龜山島":
                return "宜蘭縣";
            case "花蓮":
            case "花蓮市":
                return "花蓮縣";
            case "台東":
            case "台東市":
            case "綠島":
            case "蘭嶼":
                return "台東縣";
            case "馬祖":
            case "媽祖":
            case "連江":
                return "連江縣";
            case "金門":
            case "金門市":
                return "金門縣";
            case "澎湖":
            case "澎湖市":
                return "澎湖縣";
        }
        return null;
    }

    public static boolean inKeywords(String text, String... keywords) {
        for (String keyword : keywords) {
            text = text.toLowerCase();
            if (text.contains(keyword.toLowerCase())) {
                return true;
            }
        }
        return false;
    }

    public static boolean inKeywords(String text, ArrayList<String> keywords) {
        if (keywords == null) {
            return false;
        }
        for (String keyword : keywords) {
            if (text.contains(keyword)) {
                return true;
            }
        }
        return false;
    }
    //stupid code start

    public static HashMap<String, HashMap<String, ArrayList<String>>> readKeywordsTable(Context c) {
        HashMap<String, HashMap<String, ArrayList<String>>> keywordTable = new HashMap<String, HashMap<String, ArrayList<String>>>();
        InputStream csvInput = c.getResources().openRawResource(R.raw.keywords);
        BufferedReader reader = new BufferedReader(new InputStreamReader(csvInput));
        try {
            String word;
            while ((word = reader.readLine()) != null) {
                String[] colContain = word.split(",");
                String[] opt1 = colContain[2].split(";");
                String[] opt2 = colContain[4].split(";");
                HashMap<String, ArrayList<String>> innerHash = new HashMap<String, ArrayList<String>>();
                ArrayList<String> opt1Keys = new ArrayList<String>();
                ArrayList<String> opt2Keys = new ArrayList<String>();
                for (String k : opt1) {
                    opt1Keys.add(k);
                }
                for (String k : opt2) {
                    opt2Keys.add(k);
                }
                innerHash.put(colContain[1], opt1Keys);
                innerHash.put(colContain[3], opt2Keys);
                keywordTable.put(colContain[0], innerHash);
            }
        } catch (Exception e) {
        }
        return keywordTable;
    }
    //stupid code end
}
