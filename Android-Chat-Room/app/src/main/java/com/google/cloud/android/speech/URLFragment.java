package com.google.cloud.android.speech;

import android.content.Intent;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.URLUtil;
import android.widget.ImageButton;
import android.widget.TextView;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Random;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import retrofit2.Retrofit;

import static com.google.cloud.android.speech.Global.wrongList;
import static com.google.cloud.android.speech.MessageAdapter.getBitmapFromURL;

public class URLFragment extends SpeechFragment {
    ImageButton News1;
    ImageButton News2;
    ImageButton News3;
    ImageButton News4;
    ImageButton News5;
    TextView Title1;
    TextView Title2;
    TextView Title3;
    TextView Title4;
    TextView Title5;
    String url1;
    String image1;
    String Text1;
    String url2;
    String image2;
    String Text2;
    String url3;
    String image3;
    String Text3;
    String url4;
    String image4;
    String Text4;
    String url5;
    String image5;
    String Text5;
    //UI
    private ImageButton backBtn;



    public URLFragment() {
        // Required empty public constructor
    }

    public static URLFragment newInstance(String param1, String param2) {
        URLFragment fragment = new URLFragment();
        Bundle args = new Bundle();

        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        IP="140.116.247.169";
        thread = new Thread(Connection);     //賦予執行緒工作
        thread.start();
        mThreadPool = Executors.newCachedThreadPool();  //初始 thread pool
        if (getArguments() != null) {

        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View view = inflater.inflate(R.layout.fragment_url, container, false);
        initView(view);
        Log.d("URL", "onCreateView");
        return view;
    }

    private void initView(View view){
        Log.d("url", "initView");
        backBtn = view.findViewById(R.id.backBBB);
        backBtn.setOnClickListener(v -> {
            jumpNextFragment(new MainActivityFragment());
        });
        News1 =(ImageButton) view.findViewById(R.id.News1);
        News1.setOnClickListener(v -> {
            click1();
        });
        News2 =(ImageButton) view.findViewById(R.id.News2);
        News2.setOnClickListener(v -> {
            click2();
        });
        News3 =(ImageButton) view.findViewById(R.id.News3);
        News3.setOnClickListener(v -> {
            click3();
        });
        News4 =(ImageButton) view.findViewById(R.id.News4);
        News4.setOnClickListener(v -> {
            click4();
        });
        News5 =(ImageButton) view.findViewById(R.id.News5);
        News5.setOnClickListener(v -> {
            click5();
        });
        Title1 = (TextView)view.findViewById(R.id.Title1);
        Title2 = (TextView)view.findViewById(R.id.Title2);
        Title3 = (TextView)view.findViewById(R.id.Title3);
        Title4 = (TextView)view.findViewById(R.id.Title4);
        Title5 = (TextView)view.findViewById(R.id.Title5);


        send();
    }

    @Override
    public void onPause() {
        super.onPause();

    }

    private  Handler recMessageHandler = new Handler(){
        @Override
        public void handleMessage(Message msg)
        {
            // TODO Auto-generated method stub
            String text = (String)msg.obj;
            Log.d("iamagod",text);
            url1 = text.substring(0,text.indexOf('|'));
            Log.d("iamagod",url1);
            text = text.substring(text.indexOf('|')+1);
            image1 = text.substring(0,text.indexOf('|'));
            text = text.substring(text.indexOf('|')+1);
            Log.d("iamagod",image1);
            if(URLUtil.isValidUrl(image1)){
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        Bitmap mBitmap = getBitmapFromURL(image1);
                        getActivity().runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                News1.setImageBitmap(mBitmap);
                            }
                        });
                    }
                }).start();

            }
            Text1 =text.substring(0,text.indexOf('|'));
            Log.d("iamagod",Text1);
            text = text.substring(text.indexOf('|')+1);

            url2 = text.substring(0,text.indexOf('|'));
            Log.d("iamagod",url2);
            text = text.substring(text.indexOf('|')+1);
            image2 = text.substring(0,text.indexOf('|'));
            text = text.substring(text.indexOf('|')+1);
            Log.d("iamagod",image2);
            if(URLUtil.isValidUrl(image2)){
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        Bitmap mBitmap = getBitmapFromURL(image2);
                        getActivity().runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                News2.setImageBitmap(mBitmap);
                            }
                        });

                    }
                }).start();

            }
            Text3 =text.substring(0,text.indexOf('|'));
            Log.d("iamagod",Text3);
            text = text.substring(text.indexOf('|')+1);

            url3 = text.substring(0,text.indexOf('|'));
            Log.d("iamagod",url3);
            text = text.substring(text.indexOf('|')+1);
            image3 = text.substring(0,text.indexOf('|'));
            text = text.substring(text.indexOf('|')+1);
            Log.d("iamagod",image3);
            if(URLUtil.isValidUrl(image3)){
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        Bitmap mBitmap = getBitmapFromURL(image3);
                        getActivity().runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                News3.setImageBitmap(mBitmap);
                            }
                        });
                    }
                }).start();

            }
            Text2 =text.substring(0,text.indexOf('|'));
            Log.d("iamagod",Text2);
            text = text.substring(text.indexOf('|')+1);

            url4 = text.substring(0,text.indexOf('|'));
            Log.d("iamagod",url4);
            text = text.substring(text.indexOf('|')+1);
            image4 = text.substring(0,text.indexOf('|'));
            text = text.substring(text.indexOf('|')+1);
            Log.d("iamagod",image4);
            if(URLUtil.isValidUrl(image4)){
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        Bitmap mBitmap = getBitmapFromURL(image4);
                        getActivity().runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                News4.setImageBitmap(mBitmap);
                            }
                        });

                    }
                }).start();

            }
            Text4 =text.substring(0,text.indexOf('|'));
            Log.d("iamagod",Text4);
            text = text.substring(text.indexOf('|')+1);


            url5 = text.substring(0,text.indexOf('|'));
            Log.d("iamagod",url5);
            text = text.substring(text.indexOf('|')+1);
            image5 = text.substring(0,text.indexOf('|'));
            text = text.substring(text.indexOf('|')+1);
            Log.d("iamagod",image5);
            if(URLUtil.isValidUrl(image5)){
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        Bitmap mBitmap = getBitmapFromURL(image5);
                        getActivity().runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                News5.setImageBitmap(mBitmap);
                            }
                        });
                    }
                }).start();

            }
            Text5 =text.substring(0,text.indexOf('|'));
            Log.d("iamagod",Text5);
            text = text.substring(0,text.indexOf('|'));

            Title1.setText(Text1);
            Title2.setText(Text2);
            Title3.setText(Text3);
            Title4.setText(Text4);
            Title5.setText(Text5);

        }
    };

    private ExecutorService mThreadPool;    // thread pool
    private Thread thread;
    private Socket clientSocket;        //客戶端的socket
    private BufferedWriter bw;            //取得網路輸出串流
    private BufferedReader br;            //取得網路輸入串流
    private String tmp;                    //做為接收時的緩存
    private String IP;

    private Runnable Connection=new Runnable(){
        @Override
        public void run() {
            // TODO Auto-generated method stub
            try{
                // IP為Server端
                InetAddress serverIp = InetAddress.getByName(IP);
                int serverPort = 2005;
                clientSocket = new Socket(serverIp, serverPort);
                br = new BufferedReader(new InputStreamReader(clientSocket.getInputStream(),"UTF-8"));
                Log.d("iamagod","connection");

                // 當連線後
                while (true) {
                    // 取得網路訊息
                    tmp = br.readLine();    //宣告一個緩衝,從br串流讀取值

                    // 如果不是空訊息
                    if(tmp!=null){
                        Message msg = Message.obtain();
                        msg.obj = tmp;
                        msg.setTarget(recMessageHandler);
                        msg.sendToTarget();
                    }
                }
            }
            catch(Exception e){
                Log.d("iamagod","not");
            }
        }
    };

    public void send(){
        final String temp;
        Random ran = new Random();
        //temp=(String)mText.getText();
        if(wrongList.size() > 0){
            int a = ran.nextInt(wrongList.size());
            String ttemp = wrongList.get(a).toString();
            a = Integer.valueOf(ttemp) - 950;
            temp = Integer.toString(a);
            Log.d("iamagod",temp);
        }
        else{
            temp = Integer.toString(ran.nextInt(150)+1);
            Log.d("iamagod",temp);
        }
        mThreadPool.execute(new Runnable() {
            @Override
            public void run() {
                if(temp.length()>0)
                {
                    try{
                        //取得網路輸出串流

                        bw = new BufferedWriter( new OutputStreamWriter(clientSocket.getOutputStream(),"UTF-8"));
                        //寫入訊息
                        bw.write(temp);
                        //送出
                        bw.flush();
                    }catch(IOException e){
                    }
                }
            }
        });

    }

    @Override
    public SpeechReaction getSpeechReaction() {
        Log.d("URLFragment","getSpeechReaction");
        return new SpeechReaction() {
            @Override
            public void onSentenceEnd(String text) {
                Log.d("URLFragment","SentenceEnd");

                String[] keywordsOfHealthEdu = {"衛教", "味覺", "衛生", "教育", "題目", "題庫"};
                if(TextUtil.inKeywords(text, keywordsOfHealthEdu)){
                   // mHealthBtn.performClick();
                }
            }
        };
    }

    public void click1() {
        Intent sharingIntent = new Intent(Intent.ACTION_VIEW);
        sharingIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        sharingIntent.setData(Uri.parse(url1));
        Intent chooserIntent = Intent.createChooser(sharingIntent, "Open With");
        chooserIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        if (chooserIntent.resolveActivity(this.getActivity().getPackageManager()) != null && URLUtil.isValidUrl(url1)) {
            mThreadPool.execute(new Runnable() {
                @Override
                public void run() {
                    if(url1.length()>0)
                    {
                        try{
                            //取得網路輸出串流
                            bw = new BufferedWriter( new OutputStreamWriter(clientSocket.getOutputStream(),"UTF-8"));
                            //寫入訊息
                            bw.write("taptaptap"+url1);
                            //送出
                            bw.flush();
                            //messageHandler.sendMessage(msg);
                            //mAdapter.addResult(temp);
                            //mRecyclerView.smoothScrollToPosition(0);
                        }catch(IOException e){
                            //error
                        }
                        //清空輸入框
                        //mText.setText("");
                    }
                }
            });
            this.startActivity(chooserIntent);
        } else {
            Log.d("ImplicitIntents", "Can't handle this intent!");
        }

    }

    public void click2() {
        Intent sharingIntent = new Intent(Intent.ACTION_VIEW);
        sharingIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        sharingIntent.setData(Uri.parse(url2));
        Intent chooserIntent = Intent.createChooser(sharingIntent, "Open With");
        chooserIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        if (chooserIntent.resolveActivity(this.getActivity().getPackageManager()) != null && URLUtil.isValidUrl(url2)) {
            mThreadPool.execute(new Runnable() {
                @Override
                public void run() {
                    if(url2.length()>0)
                    {
                        try{
                            //取得網路輸出串流
                            bw = new BufferedWriter( new OutputStreamWriter(clientSocket.getOutputStream(),"UTF-8"));
                            //寫入訊息
                            bw.write("taptaptap"+url2);
                            //送出
                            bw.flush();
                            //messageHandler.sendMessage(msg);
                            //mAdapter.addResult(temp);
                            //mRecyclerView.smoothScrollToPosition(0);
                        }catch(IOException e){
                            //error
                        }
                        //清空輸入框
                        //mText.setText("");
                    }
                }
            });
            this.startActivity(chooserIntent);
        } else {
            Log.d("ImplicitIntents", "Can't handle this intent!");
        }
    }

    public void click3() {
        Intent sharingIntent = new Intent(Intent.ACTION_VIEW);
        sharingIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        sharingIntent.setData(Uri.parse(url3));
        Intent chooserIntent = Intent.createChooser(sharingIntent, "Open With");
        chooserIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        if (chooserIntent.resolveActivity(this.getActivity().getPackageManager()) != null && URLUtil.isValidUrl(url3)) {
            mThreadPool.execute(new Runnable() {
                @Override
                public void run() {
                    if(url3.length()>0)
                    {
                        try{
                            //取得網路輸出串流
                            bw = new BufferedWriter( new OutputStreamWriter(clientSocket.getOutputStream(),"UTF-8"));
                            //寫入訊息
                            bw.write("taptaptap"+url3);
                            //送出
                            bw.flush();
                            //messageHandler.sendMessage(msg);
                            //mAdapter.addResult(temp);
                            //mRecyclerView.smoothScrollToPosition(0);
                        }catch(IOException e){
                            //error
                        }
                        //清空輸入框
                        //mText.setText("");
                    }
                }
            });
            this.startActivity(chooserIntent);
        } else {
            Log.d("ImplicitIntents", "Can't handle this intent!");
        }
    }


    public void click4() {
        Intent sharingIntent = new Intent(Intent.ACTION_VIEW);
        sharingIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        sharingIntent.setData(Uri.parse(url4));
        Intent chooserIntent = Intent.createChooser(sharingIntent, "Open With");
        chooserIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        if (chooserIntent.resolveActivity(this.getActivity().getPackageManager()) != null && URLUtil.isValidUrl(url4)) {
            mThreadPool.execute(new Runnable() {
                @Override
                public void run() {
                    if(url4.length()>0)
                    {
                        try{
                            //取得網路輸出串流
                            bw = new BufferedWriter( new OutputStreamWriter(clientSocket.getOutputStream(),"UTF-8"));
                            //寫入訊息
                            bw.write("taptaptap"+url4);
                            //送出
                            bw.flush();
                            //messageHandler.sendMessage(msg);
                            //mAdapter.addResult(temp);
                            //mRecyclerView.smoothScrollToPosition(0);
                        }catch(IOException e){
                            //error
                        }
                        //清空輸入框
                        //mText.setText("");
                    }
                }
            });
            this.startActivity(chooserIntent);
        } else {
            Log.d("ImplicitIntents", "Can't handle this intent!");
        }
    }

    public void click5() {
        Intent sharingIntent = new Intent(Intent.ACTION_VIEW);
        sharingIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        sharingIntent.setData(Uri.parse(url5));
        Intent chooserIntent = Intent.createChooser(sharingIntent, "Open With");
        chooserIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        if (chooserIntent.resolveActivity(getActivity().getPackageManager()) != null && URLUtil.isValidUrl(url5)) {
            mThreadPool.execute(new Runnable() {
                @Override
                public void run() {
                    if(url5.length()>0)
                    {
                        try{
                            //取得網路輸出串流
                            bw = new BufferedWriter( new OutputStreamWriter(clientSocket.getOutputStream(),"UTF-8"));
                            //寫入訊息
                            bw.write("taptaptap"+url5);
                            //送出
                            bw.flush();
                            //messageHandler.sendMessage(msg);
                            //mAdapter.addResult(temp);
                            //mRecyclerView.smoothScrollToPosition(0);
                        }catch(IOException e){
                            //error
                        }
                        //清空輸入框
                        //mText.setText("");
                    }
                }
            });
            this.startActivity(chooserIntent);
        } else {
            Log.d("ImplicitIntents", "Can't handle this intent!");
        }
    }
}
