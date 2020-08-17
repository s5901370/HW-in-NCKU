//package com.google.cloud.android.speech;
//
//import android.content.Intent;
//import android.graphics.Bitmap;
//import android.graphics.Color;
//import android.graphics.Paint;
//import android.net.Uri;
//import android.os.Message;
//import android.support.v7.app.AppCompatActivity;
//import android.os.Bundle;
//import android.util.Log;
//import android.view.View;
//import android.webkit.URLUtil;
//import android.widget.ImageButton;
//import android.widget.TextView;
//
//import java.io.BufferedReader;
//import java.io.BufferedWriter;
//import java.io.IOException;
//import java.io.InputStreamReader;
//import java.io.OutputStreamWriter;
//import java.net.InetAddress;
//import java.net.Socket;
//import java.net.UnknownHostException;
//import java.util.concurrent.ExecutorService;
//import java.util.concurrent.Executors;
//
//import static com.google.cloud.android.speech.MessageAdapter.getBitmapFromURL;
//
//public class LayoutActivity extends AppCompatActivity {
//    private ExecutorService mThreadPool;
//    private Thread thread;
//    private Socket clientSocket;
//    private BufferedWriter bw;
//    String IP="140.116.247.169";
//    ImageButton News1;
//    ImageButton News2;
//    ImageButton News3;
//    ImageButton News4;
//    ImageButton News5;
//    TextView Title1;
//    TextView Title2;
//    TextView Title3;
//    TextView Title4;
//    TextView Title5;
//    String url1;
//    String image1;
//    String Text1;
//    String url2;
//    String image2;
//    String Text2;
//    String url3;
//    String image3;
//    String Text3;
//    String url4;
//    String image4;
//    String Text4;
//    String url5;
//    String image5;
//    String Text5;
//    @Override
//    protected void onCreate(Bundle savedInstanceState) {
//        super.onCreate(savedInstanceState);
//        setContentView(R.layout.activity_layout);
//        Intent intent = getIntent();
//        News1 =(ImageButton) findViewById(R.id.News1);
//        News2 =(ImageButton) findViewById(R.id.News2);
//        News3 =(ImageButton) findViewById(R.id.News3);
//        News4 =(ImageButton) findViewById(R.id.News4);
//        News5 =(ImageButton) findViewById(R.id.News5);
//        Title1 = (TextView)findViewById(R.id.Title1);
//        Title2 = (TextView)findViewById(R.id.Title2);
//        Title3 = (TextView)findViewById(R.id.Title3);
//        Title4 = (TextView)findViewById(R.id.Title4);
//        Title5 = (TextView)findViewById(R.id.Title5);
//        String text = intent.getStringExtra(MainActivity.NEWS);
//
//        url1 = text.substring(0,text.indexOf('|'));
//        Log.d("iamagod",url1);
//        text = text.substring(text.indexOf('|')+1);
//        image1 = text.substring(0,text.indexOf('|'));
//        text = text.substring(text.indexOf('|')+1);
//        Log.d("iamagod",image1);
//        if(URLUtil.isValidUrl(image1)){
//            new Thread(new Runnable() {
//                @Override
//                public void run() {
//                    final Bitmap mBitmap = getBitmapFromURL(image1);
//                    runOnUiThread(new Runnable() {
//                        @Override
//                        public void run() {
//                            News1.setImageBitmap(mBitmap);
//                        }
//                    });
//                }
//            }).start();
//
//        }
//        Text1 =text.substring(0,text.indexOf('|'));
//        Log.d("iamagod",Text1);
//        text = text.substring(text.indexOf('|')+1);
//
//        url2 = text.substring(0,text.indexOf('|'));
//        Log.d("iamagod",url2);
//        text = text.substring(text.indexOf('|')+1);
//        image2 = text.substring(0,text.indexOf('|'));
//        text = text.substring(text.indexOf('|')+1);
//        Log.d("iamagod",image2);
//        if(URLUtil.isValidUrl(image2)){
//            new Thread(new Runnable() {
//                @Override
//                public void run() {
//                    final Bitmap mBitmap = getBitmapFromURL(image2);
//                    runOnUiThread(new Runnable() {
//                        @Override
//                        public void run() {
//                            News2.setImageBitmap(mBitmap);
//                        }
//                    });
//                }
//            }).start();
//
//        }
//        Text3 =text.substring(0,text.indexOf('|'));
//        Log.d("iamagod",Text3);
//        text = text.substring(text.indexOf('|')+1);
//
//        url3 = text.substring(0,text.indexOf('|'));
//        Log.d("iamagod",url3);
//        text = text.substring(text.indexOf('|')+1);
//        image3 = text.substring(0,text.indexOf('|'));
//        text = text.substring(text.indexOf('|')+1);
//        Log.d("iamagod",image3);
//        if(URLUtil.isValidUrl(image3)){
//            new Thread(new Runnable() {
//                @Override
//                public void run() {
//                    final Bitmap mBitmap = getBitmapFromURL(image3);
//                    runOnUiThread(new Runnable() {
//                        @Override
//                        public void run() {
//                            News3.setImageBitmap(mBitmap);
//                        }
//                    });
//                }
//            }).start();
//
//        }
//        Text2 =text.substring(0,text.indexOf('|'));
//        Log.d("iamagod",Text2);
//        text = text.substring(text.indexOf('|')+1);
//
//        url4 = text.substring(0,text.indexOf('|'));
//        Log.d("iamagod",url4);
//        text = text.substring(text.indexOf('|')+1);
//        image4 = text.substring(0,text.indexOf('|'));
//        text = text.substring(text.indexOf('|')+1);
//        Log.d("iamagod",image4);
//        if(URLUtil.isValidUrl(image4)){
//            new Thread(new Runnable() {
//                @Override
//                public void run() {
//                    final Bitmap mBitmap = getBitmapFromURL(image4);
//                    runOnUiThread(new Runnable() {
//                        @Override
//                        public void run() {
//                            News4.setImageBitmap(mBitmap);
//                        }
//                    });
//                }
//            }).start();
//
//        }
//        Text4 =text.substring(0,text.indexOf('|'));
//        Log.d("iamagod",Text4);
//        text = text.substring(text.indexOf('|')+1);
//
//
//        url5 = text.substring(0,text.indexOf('|'));
//        Log.d("iamagod",url5);
//        text = text.substring(text.indexOf('|')+1);
//        image5 = text.substring(0,text.indexOf('|'));
//        text = text.substring(text.indexOf('|')+1);
//        Log.d("iamagod",image5);
//        if(URLUtil.isValidUrl(image5)){
//            new Thread(new Runnable() {
//                @Override
//                public void run() {
//                    final Bitmap mBitmap = getBitmapFromURL(image5);
//                    runOnUiThread(new Runnable() {
//                        @Override
//                        public void run() {
//                            News5.setImageBitmap(mBitmap);
//                        }
//                    });
//                }
//            }).start();
//
//        }
//        Text5 =text.substring(0,text.indexOf('|'));
//        Log.d("iamagod",Text5);
//        text = text.substring(0,text.indexOf('|'));
//
//        Title1.setText(Text1);
//        Title2.setText(Text2);
//        Title3.setText(Text3);
//        Title4.setText(Text4);
//        Title5.setText(Text5);
//        thread = new Thread(Connection);     //賦予執行緒工作
//        thread.start();
//        mThreadPool = Executors.newCachedThreadPool();  //初始 thread pool
//
//    }
//    private Runnable Connection=new Runnable(){
//        @Override
//        public void run() {
//            // TODO Auto-generated method stub
//            try{
//                // IP為Server端
//                InetAddress serverIp = InetAddress.getByName(IP);
//                int serverPort = 2005;
//                clientSocket = new Socket(serverIp, serverPort);
//                Log.d("iamagod","client");
//            }
//            catch(Exception e){
//                //error
//            }
//        }
//    };
//    public void click1(View view) {
//        Intent sharingIntent = new Intent(Intent.ACTION_VIEW);
//        sharingIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
//        sharingIntent.setData(Uri.parse(url1));
//        Intent chooserIntent = Intent.createChooser(sharingIntent, "Open With");
//        chooserIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
//        if (chooserIntent.resolveActivity(this.getPackageManager()) != null && URLUtil.isValidUrl(url1)) {
//            mThreadPool.execute(new Runnable() {
//                @Override
//                public void run() {
//                    if(url1.length()>0)
//                    {
//                        try{
//                            //取得網路輸出串流
//                            bw = new BufferedWriter( new OutputStreamWriter(clientSocket.getOutputStream(),"UTF-8"));
//                            //寫入訊息
//                            bw.write("taptaptap"+url1);
//                            //送出
//                            bw.flush();
//                            //messageHandler.sendMessage(msg);
//                            //mAdapter.addResult(temp);
//                            //mRecyclerView.smoothScrollToPosition(0);
//                        }catch(IOException e){
//                            //error
//                        }
//                        //清空輸入框
//                        //mText.setText("");
//                    }
//                }
//            });
//            this.startActivity(chooserIntent);
//        } else {
//            Log.d("ImplicitIntents", "Can't handle this intent!");
//        }
//
//    }
//
//    public void click2(View view) {
//        Intent sharingIntent = new Intent(Intent.ACTION_VIEW);
//        sharingIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
//        sharingIntent.setData(Uri.parse(url2));
//        Intent chooserIntent = Intent.createChooser(sharingIntent, "Open With");
//        chooserIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
//        if (chooserIntent.resolveActivity(this.getPackageManager()) != null && URLUtil.isValidUrl(url2)) {
//            mThreadPool.execute(new Runnable() {
//                @Override
//                public void run() {
//                    if(url2.length()>0)
//                    {
//                        try{
//                            //取得網路輸出串流
//                            bw = new BufferedWriter( new OutputStreamWriter(clientSocket.getOutputStream(),"UTF-8"));
//                            //寫入訊息
//                            bw.write("taptaptap"+url2);
//                            //送出
//                            bw.flush();
//                            //messageHandler.sendMessage(msg);
//                            //mAdapter.addResult(temp);
//                            //mRecyclerView.smoothScrollToPosition(0);
//                        }catch(IOException e){
//                            //error
//                        }
//                        //清空輸入框
//                        //mText.setText("");
//                    }
//                }
//            });
//            this.startActivity(chooserIntent);
//        } else {
//            Log.d("ImplicitIntents", "Can't handle this intent!");
//        }
//    }
//
//    public void click3(View view) {
//        Intent sharingIntent = new Intent(Intent.ACTION_VIEW);
//        sharingIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
//        sharingIntent.setData(Uri.parse(url3));
//        Intent chooserIntent = Intent.createChooser(sharingIntent, "Open With");
//        chooserIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
//        if (chooserIntent.resolveActivity(this.getPackageManager()) != null && URLUtil.isValidUrl(url3)) {
//            mThreadPool.execute(new Runnable() {
//                @Override
//                public void run() {
//                    if(url3.length()>0)
//                    {
//                        try{
//                            //取得網路輸出串流
//                            bw = new BufferedWriter( new OutputStreamWriter(clientSocket.getOutputStream(),"UTF-8"));
//                            //寫入訊息
//                            bw.write("taptaptap"+url3);
//                            //送出
//                            bw.flush();
//                            //messageHandler.sendMessage(msg);
//                            //mAdapter.addResult(temp);
//                            //mRecyclerView.smoothScrollToPosition(0);
//                        }catch(IOException e){
//                            //error
//                        }
//                        //清空輸入框
//                        //mText.setText("");
//                    }
//                }
//            });
//            this.startActivity(chooserIntent);
//        } else {
//            Log.d("ImplicitIntents", "Can't handle this intent!");
//        }
//    }
//
//    public void back(View view) {
//        Log.d("iamagod","back");
//        finish();
//
//    }
//
//    public void click4(View view) {
//        Intent sharingIntent = new Intent(Intent.ACTION_VIEW);
//        sharingIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
//        sharingIntent.setData(Uri.parse(url4));
//        Intent chooserIntent = Intent.createChooser(sharingIntent, "Open With");
//        chooserIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
//        if (chooserIntent.resolveActivity(this.getPackageManager()) != null && URLUtil.isValidUrl(url4)) {
//            mThreadPool.execute(new Runnable() {
//                @Override
//                public void run() {
//                    if(url4.length()>0)
//                    {
//                        try{
//                            //取得網路輸出串流
//                            bw = new BufferedWriter( new OutputStreamWriter(clientSocket.getOutputStream(),"UTF-8"));
//                            //寫入訊息
//                            bw.write("taptaptap"+url4);
//                            //送出
//                            bw.flush();
//                            //messageHandler.sendMessage(msg);
//                            //mAdapter.addResult(temp);
//                            //mRecyclerView.smoothScrollToPosition(0);
//                        }catch(IOException e){
//                            //error
//                        }
//                        //清空輸入框
//                        //mText.setText("");
//                    }
//                }
//            });
//            this.startActivity(chooserIntent);
//        } else {
//            Log.d("ImplicitIntents", "Can't handle this intent!");
//        }
//    }
//
//    public void click5(View view) {
//        Intent sharingIntent = new Intent(Intent.ACTION_VIEW);
//        sharingIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
//        sharingIntent.setData(Uri.parse(url5));
//        Intent chooserIntent = Intent.createChooser(sharingIntent, "Open With");
//        chooserIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
//        if (chooserIntent.resolveActivity(this.getPackageManager()) != null && URLUtil.isValidUrl(url5)) {
//            mThreadPool.execute(new Runnable() {
//                @Override
//                public void run() {
//                    if(url5.length()>0)
//                    {
//                        try{
//                            //取得網路輸出串流
//                            bw = new BufferedWriter( new OutputStreamWriter(clientSocket.getOutputStream(),"UTF-8"));
//                            //寫入訊息
//                            bw.write("taptaptap"+url5);
//                            //送出
//                            bw.flush();
//                            //messageHandler.sendMessage(msg);
//                            //mAdapter.addResult(temp);
//                            //mRecyclerView.smoothScrollToPosition(0);
//                        }catch(IOException e){
//                            //error
//                        }
//                        //清空輸入框
//                        //mText.setText("");
//                    }
//                }
//            });
//            this.startActivity(chooserIntent);
//        } else {
//            Log.d("ImplicitIntents", "Can't handle this intent!");
//        }
//    }
//}
