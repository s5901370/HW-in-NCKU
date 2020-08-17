package com.google.cloud.android.speech;


import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.drawable.GradientDrawable;
import android.os.Handler;
import android.support.annotation.NonNull;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.URLUtil;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Member;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;

import static android.view.LayoutInflater.from;

public class MessageAdapter extends RecyclerView.Adapter<RecyclerView.ViewHolder> {

    List<MyMessage> messages = new ArrayList<MyMessage>();
    Context context;
    private boolean isServer;
    private final Handler handler;
    int dataSize;

    public MessageAdapter(Context context) {
        this.context = context;
        this.isServer = false;
        handler = new Handler(context.getMainLooper());
//        this.messages.addAll(arrayList);
    }

    public void add(MyMessage message) {

        this.messages.add(message);
        notifyDataSetChanged(); // to render the list we need to notify
    }
    public List<MyMessage> getResult(){
        return messages;
    }


    @Override
    public RecyclerView.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View view;
        if (viewType == 0) { // for call layout
             view = LayoutInflater.from(context).inflate(R.layout.my_message, parent, false);
            return new zeroHolder(view);
        }
        else if(viewType == 1){
            view = LayoutInflater.from(context).inflate(R.layout.other_message, parent, false);
            return new oneHolder(view);
        }
        else if(viewType == 2){
            view = LayoutInflater.from(context).inflate(R.layout.another_message, parent, false);
            return new twoHolder(view);
        }
        else{// type == 3
            view = LayoutInflater.from(context).inflate(R.layout.list_item, parent, false);
            return new threeHolder(view);
        }
    }

    @Override
    public void onBindViewHolder(RecyclerView.ViewHolder holder, int position) {
        if (getItemViewType(position) == 0) {
            ((zeroHolder) holder).set(messages.get(position));
        }
        else if(getItemViewType(position) == 1){
            ((oneHolder) holder).set(messages.get(position));
        }
        else if(getItemViewType(position) == 2){
            ((twoHolder) holder).set(messages.get(position));
        }
        else if(getItemViewType(position) == 3){
            ((threeHolder) holder).set(messages.get(position));
        }
    }

    @Override
    public long getItemId(int i) {
        return i;
    }

    @Override
    public int getItemCount() {
        return messages.size();
    }

    @Override
    public int getItemViewType(int position) {
        if(messages.get(position).isBelongsToCurrentUser() == 1){
            return 1;
        }
        else if (messages.get(position).isBelongsToCurrentUser() == 2){
            return 2;
        }
        else if (messages.get(position).isBelongsToCurrentUser() == 3){
            return 3;
        }
        else{ //if (messages.get(position).isBelongsToCurrentUser() == 0){
            return 0;
        }
    }

    class zeroHolder extends RecyclerView.ViewHolder{// this message was sent by us so let's create a basic chat bubble on the right
        TextView messageBody;
        TextView time;
        public zeroHolder(@NonNull View itemView) {
            super(itemView);
            messageBody  = (TextView) itemView.findViewById(R.id.message_body);
            time = (TextView) itemView.findViewById(R.id.time);
        }

        private void set(MyMessage message){
            messageBody.setText(message.getText());
            time.setText(message.getTime());
        }
    }

    class oneHolder extends RecyclerView.ViewHolder{
        TextView messageBody;
        TextView time;
        public oneHolder(@NonNull View itemView) {
            super(itemView);
            messageBody  = (TextView) itemView.findViewById(R.id.message_body);
            time = (TextView) itemView.findViewById(R.id.time);
        }

        private void set(MyMessage message){
            messageBody.setText(message.getText());
            if(URLUtil.isValidUrl(message.getText())){
                messageBody.setPaintFlags(messageBody.getPaintFlags() |   Paint.UNDERLINE_TEXT_FLAG);
                messageBody.setTextColor(Color.parseColor("#87CEEB"));

            }
            time.setText(message.getTime());
        }
    }

    class twoHolder extends RecyclerView.ViewHolder{ // this message was sent by someone else so let's create an advanced chat bubble on the left
        TextView messageBody;
        TextView time;
        TextView name ;
        ImageView avatar;
        public twoHolder(@NonNull View itemView) {
            super(itemView);
            messageBody  = (TextView) itemView.findViewById(R.id.message_body);
            time = (TextView) itemView.findViewById(R.id.time);
            name = (TextView) itemView.findViewById(R.id.name);
            avatar = (ImageView) itemView.findViewById(R.id.avatar);
        }

        private void set(MyMessage message){
            messageBody.setText(message.getText());
            if(URLUtil.isValidUrl(message.getText())){
                messageBody.setPaintFlags(messageBody.getPaintFlags() |   Paint.UNDERLINE_TEXT_FLAG);
                messageBody.setTextColor(Color.parseColor("#87CEEB"));
            }
            time.setText(message.getTime());
        }
    }

    class threeHolder extends RecyclerView.ViewHolder{
//        TextView messageBody;
        TextView time;
//        TextView name ;
//        ImageView avatar;
        RecyclerView recyclerView ;
        public threeHolder(@NonNull View itemView) {
            super(itemView);
//            messageBody  = (TextView) itemView.findViewById(R.id.message_body);
            time = (TextView) itemView.findViewById(R.id.time);
//            name = (TextView) itemView.findViewById(R.id.name);
//            avatar = (ImageView) itemView.findViewById(R.id.avatar);
            recyclerView = (RecyclerView) itemView.findViewById(R.id.recycler_view_list);
        }
        private void set(MyMessage message){
            if(message.getText().indexOf('|') > 0){
                ArrayList<MemberData> List = new ArrayList<MemberData>();
                String text = message.getText();
                for(int i=0;i<2;++i){
                    String url = text.substring(0,text.indexOf('|'));
                    Log.d("iamagod",url);
                    text = text.substring(text.indexOf('|')+1);
                    final String image = text.substring(0,text.indexOf('|'));
                    text = text.substring(text.indexOf('|')+1);
                    Log.d("iamagod",image);
                    String title =text.substring(0,text.indexOf('|'));
                    Log.d("iamagod",title);
                    List.add(new MemberData(image,url,title));
                    text = text.substring(text.indexOf('|')+1);
                }
                String url = text.substring(0,text.indexOf('|'));
                Log.d("iamagod",url);
                text = text.substring(text.indexOf('|')+1);
                final String image = text.substring(0,text.indexOf('|'));
                text = text.substring(text.indexOf('|')+1);
                Log.d("iamagod",image);

                String title =text.substring(0,text.indexOf('|'));
                Log.d("iamagod",title);
                List.add(new MemberData(image,url,title));


                ListAdapter adapter = new ListAdapter(context,List);
                recyclerView.setLayoutManager(new LinearLayoutManager(context, LinearLayoutManager.HORIZONTAL, false));
                recyclerView.setAdapter(adapter);
//                if(URLUtil.isValidUrl(url)){
//                    name.setText(url);
//                    name.setPaintFlags(messageBody.getPaintFlags() |   Paint.UNDERLINE_TEXT_FLAG);
//                    name.setTextColor(Color.parseColor("#87CEEB"));
//                }
//                if(URLUtil.isValidUrl(image)){
//                    new Thread(new Runnable() {
//                        @Override
//                        public void run() {
//                            final Bitmap mBitmap = getBitmapFromURL(image);
//                            runOnUiThread(new Runnable() {
//                                @Override
//                                public void run() {
//                                    avatar.setImageBitmap(mBitmap);
//                                }
//                            });
//                        }
//                    }).start();
//
//                }

            }

            time.setText(message.getTime());
        }

    }

    // This is the backbone of the class, it handles the creation of single ListView row (chat bubble)
//    @Override
//    public View getView(int i, View convertView, ViewGroup viewGroup) {
//        final MessageViewHolder holder = new MessageViewHolder();
//        LayoutInflater messageInflater = (LayoutInflater) context.getSystemService(Activity.LAYOUT_INFLATER_SERVICE);
//        MyMessage message = messages.get(i);
//
//        if (message.isBelongsToCurrentUser() == 0) { // this message was sent by us so let's create a basic chat bubble on the right
//            convertView = messageInflater.inflate(R.layout.my_message, null);
//            holder.messageBody = (TextView) convertView.findViewById(R.id.message_body);
//            holder.time = (TextView) convertView.findViewById(R.id.time);
//            convertView.setTag(holder);
//            holder.messageBody.setText(message.getText());
//            holder.time.setText(message.getTime());
//            isServer = false;
//        } else { // this message was sent by someone else so let's create an advanced chat bubble on the left
//            if(message.isBelongsToCurrentUser() == 2){
//                convertView = messageInflater.inflate(R.layout.another_message, null);
//                holder.messageBody = (TextView) convertView.findViewById(R.id.message_body);
//                holder.time = (TextView) convertView.findViewById(R.id.time);
//                convertView.setTag(holder);
//                holder.messageBody.setText(message.getText());
//                if(URLUtil.isValidUrl(message.getText())){
//                    holder.messageBody.setPaintFlags(holder.messageBody.getPaintFlags() |   Paint.UNDERLINE_TEXT_FLAG);
//                    holder.messageBody.setTextColor(Color.parseColor("#87CEEB"));
//                }
//                holder.time.setText(message.getTime());
//            }
//            else if (message.isBelongsToCurrentUser() == 1){
//                convertView = messageInflater.inflate(R.layout.other_message, null);
//                holder.avatar = (ImageView) convertView.findViewById(R.id.avatar);
//                holder.name = (TextView) convertView.findViewById(R.id.name);
//                holder.messageBody = (TextView) convertView.findViewById(R.id.message_body);
//                holder.time = (TextView) convertView.findViewById(R.id.time);
//                convertView.setTag(holder);
//                holder.messageBody.setText(message.getText());
//                if(URLUtil.isValidUrl(message.getText())){
//                    holder.messageBody.setPaintFlags(holder.messageBody.getPaintFlags() |   Paint.UNDERLINE_TEXT_FLAG);
//                    holder.messageBody.setTextColor(Color.parseColor("#87CEEB"));
//
//                }
//                holder.time.setText(message.getTime());
//            }
//            else if (message.isBelongsToCurrentUser() == 3){
//                convertView = messageInflater.inflate(R.layout.last_message, null);
//                holder.avatar = (ImageView) convertView.findViewById(R.id.avatar);
//                holder.name = (TextView) convertView.findViewById(R.id.name);
//                holder.messageBody = (TextView) convertView.findViewById(R.id.message_body);
//                holder.time = (TextView) convertView.findViewById(R.id.time);
//                convertView.setTag(holder);
//                if(message.getText().indexOf('|') > 0){
//                    String url = message.getText().substring(0,message.getText().indexOf('|'));
//                    final String image = message.getText().substring(message.getText().indexOf('|')+1,message.getText().lastIndexOf('|'));
//                    String title = message.getText().substring(message.getText().lastIndexOf('|')+1);
//                    if(URLUtil.isValidUrl(url)){
//                        holder.name.setText(url);
//                        holder.name.setPaintFlags(holder.messageBody.getPaintFlags() |   Paint.UNDERLINE_TEXT_FLAG);
//                        holder.name.setTextColor(Color.parseColor("#87CEEB"));
//                    }
//                    if(URLUtil.isValidUrl(image)){
//                        new Thread(new Runnable() {
//                            @Override
//                            public void run() {
//                                final Bitmap mBitmap = getBitmapFromURL(image);
//                                runOnUiThread(new Runnable() {
//                                    @Override
//                                    public void run() {
//                                        holder.avatar.setImageBitmap(mBitmap);
//                                    }
//                                });
//                            }
//                        }).start();
//
//
//
//                    }
//
//
//                    holder.messageBody.setText(title);
//                }
//
//                holder.time.setText(message.getTime());
//            }
//
//            isServer = true;
//        }
//
//        return convertView;
//    }

    private void runOnUiThread(Runnable r) {
        handler.post(r);
    }

    public static Bitmap getBitmapFromURL(String src){
        try{
            URL url = new URL(src);
            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
            conn.connect();

            InputStream input = conn.getInputStream();
            Bitmap mMap = BitmapFactory.decodeStream(input);
            return  mMap;
        }catch (IOException e){
            e.printStackTrace();
        }
        return null;
    }
}
