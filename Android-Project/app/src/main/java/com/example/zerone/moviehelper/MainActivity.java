package com.example.zerone.moviehelper;

import android.content.res.TypedArray;
import android.os.AsyncTask;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Spinner;
import android.widget.Toast;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.Iterator;

public class MainActivity extends AppCompatActivity {
    private RecyclerView mRecyclerView;
    private ArrayList<Theatre> mTheatreData;
    private myAdapter mAdapter;
    private String[] movieList; // used to store movie name
    private String[] findList;
    private String choosedMovie;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mRecyclerView = findViewById(R.id.recyclerview);
        mRecyclerView.setLayoutManager(new LinearLayoutManager(this));
        mTheatreData = new ArrayList<>();
        mAdapter = new myAdapter(this,mTheatreData);
        mRecyclerView.setAdapter(mAdapter);
        Spinner spinner = (Spinner)findViewById(R.id.spinner);
        movieList = new String[] {"MIB星際戰警：跨國行動", "地獄怪客:血后的崛起(輔15)","哥吉拉II怪獸之王Godzilla: King of the Monsters","凱蘭迪亞傳奇","樂高玩電影2(護)","蜘蛛人:新宇宙(普)","(數位版)哥吉拉II怪獸之王"};
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this,
                android.R.layout.simple_dropdown_item_1line,movieList);
        //https://developer.android.com/reference/android/R.layout.html#simple_spinner_dropdown_item  there are a lot of different type
        //adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spinner.setAdapter(adapter);
        spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                Toast.makeText(MainActivity.this, "你選的是" + movieList[position],
                        Toast.LENGTH_SHORT).show();
                choosedMovie = movieList[position];
                new FetchMovie().execute(choosedMovie);
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });
    }

    public class FetchMovie extends AsyncTask<String, Void, String> {

        @Override
        protected void onPostExecute(String s) {

            super.onPostExecute(s);
//            Toast.makeText(MainActivity.this,s,Toast.LENGTH_LONG).show();
            ArrayList<String> a =new ArrayList<String>(); //把找到的電影院add進ArrayList
            ArrayList<Iterator<String>> dates = new ArrayList<>();
            //在此parse訊息
            try {
                JSONObject jsonObject = new JSONObject(s);
                Iterator<String> keys = jsonObject.keys();
                while (keys.hasNext()){
                    String key = keys.next();
                    a.add(key);
                    JSONObject j = jsonObject.getJSONObject(key);
                    dates.add(j.keys());
                }
            }catch (JSONException e){
                e.printStackTrace();
            }

            initializeData(a,dates);
        }

        @Override
        protected String doInBackground(String... strings) {
            return Network.getTheatre(strings[0]);
        }
    }

    private void initializeData(ArrayList<String> theatres, ArrayList<Iterator<String>> dates) {
        String[] nameList = getResources().getStringArray(R.array.theatre);
        String[] addressList = getResources().getStringArray(R.array.address);
        String[] passWord = {"FE","Mall","Ambassador","Nantai","Today","beauty"};
        TypedArray iamgeList = getResources().obtainTypedArray(R.array.images);
        mTheatreData.clear();
        int index = 0;
        for(int i = 0;i<nameList.length;++i){
            if(theatres.contains(passWord[i])) {
                index = theatres.indexOf(passWord[i]);
                mTheatreData.add(new Theatre(nameList[i], addressList[i],
                        iamgeList.getResourceId(i, 0), dates.get(index)));
            }

        }
        iamgeList.recycle();
        mAdapter.notifyDataSetChanged();
    }
}
