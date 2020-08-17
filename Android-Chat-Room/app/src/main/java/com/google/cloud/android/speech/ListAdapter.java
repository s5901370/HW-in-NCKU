package com.google.cloud.android.speech;

import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.Paint;
import android.net.Uri;
import android.os.Handler;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.URLUtil;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;

import static com.google.cloud.android.speech.MessageAdapter.getBitmapFromURL;

public class ListAdapter extends RecyclerView.Adapter<ListAdapter.ListHolder> {
    private ArrayList<MemberData> List;
    private Context mContext;
    private final Handler handler;
    public ListAdapter(Context mContext,ArrayList<MemberData> list){
        this.mContext = mContext;
        this.List = list;
        handler = new Handler(mContext.getMainLooper());
    }
    @Override
    public ListAdapter.ListHolder onCreateViewHolder(ViewGroup parent, int viewType) {
       View view = LayoutInflater.from(mContext).inflate(R.layout.last_message, parent, false);
        return new ListHolder(view);
    }

    @Override
    public void onBindViewHolder(final ListAdapter.ListHolder holder, int position) {
        MemberData data = List.get(position);
        String url = data.getUrl();
        final String image = data.getImage();
        if(URLUtil.isValidUrl(url)){
            holder.name.setText(url);
            holder.name.setPaintFlags(holder.messageBody.getPaintFlags() |   Paint.UNDERLINE_TEXT_FLAG);
            holder.name.setTextColor(Color.parseColor("#87CEEB"));
            holder.name.setBackgroundColor(Color.parseColor("#ffffff"));
        }
        if(URLUtil.isValidUrl(image)){
            new Thread(new Runnable() {
                @Override
                public void run() {
                    final Bitmap mBitmap = getBitmapFromURL(image);
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            holder.avatar.setImageBitmap(mBitmap);
                        }
                    });
                }
            }).start();

        }
        holder.messageBody.setText(data.getTitle());
    }

    @Override
    public int getItemCount() {
        return (null != List ? List.size() : 0);
    }

    public class ListHolder extends RecyclerView.ViewHolder{
        protected TextView messageBody;
        protected TextView name ;
        protected ImageView avatar;
        public ListHolder(View itemView) {
            super(itemView);
            messageBody = (TextView) itemView.findViewById(R.id.message_body);
            name = (TextView)itemView.findViewById(R.id.name);
            avatar = (ImageView) itemView.findViewById(R.id.avatar);
            itemView.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    TextView textView = (TextView) view.findViewById(R.id.name);
                    String text = textView.getText().toString();
                    Intent sharingIntent = new Intent(Intent.ACTION_VIEW);
                    sharingIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                    sharingIntent.setData(Uri.parse(text));
                    Intent chooserIntent = Intent.createChooser(sharingIntent, "Open With");
                    chooserIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                    if (chooserIntent.resolveActivity(mContext.getPackageManager()) != null && URLUtil.isValidUrl(text)) {
                        mContext.startActivity(chooserIntent);
                    } else {
                        Log.d("ImplicitIntents", "Can't handle this intent!");
                    }
                }
            });


        }
    }

    private void runOnUiThread(Runnable r) {
        handler.post(r);
    }
}
