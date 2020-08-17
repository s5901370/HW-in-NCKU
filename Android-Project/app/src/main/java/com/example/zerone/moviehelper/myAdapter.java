package com.example.zerone.moviehelper;

import android.content.Context;
import android.content.Intent;
import android.support.annotation.NonNull;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import com.bumptech.glide.Glide;

import org.w3c.dom.Text;

import java.util.ArrayList;

public class myAdapter extends RecyclerView.Adapter<myAdapter.ViewHolder> {

    private ArrayList<Theatre> mTheatreData;
    private Context mContext;


    myAdapter(Context context,ArrayList<Theatre> theaterData){
        this.mTheatreData = theaterData;
        this.mContext = context;
    }
    @NonNull
    @Override
    public ViewHolder onCreateViewHolder(@NonNull ViewGroup viewGroup, int i) {
        return new ViewHolder(LayoutInflater.from(mContext).
                inflate(R.layout.list_item, viewGroup, false));
    }

    @Override
    public void onBindViewHolder(@NonNull ViewHolder viewHolder, int i) {
        Theatre current = mTheatreData.get(i);

        // Populate the textviews with data.
        viewHolder.bindTo(current);
    }

    @Override
    public int getItemCount() {
        return mTheatreData.size();
    }

    class ViewHolder extends RecyclerView.ViewHolder implements View.OnClickListener
            {

        // Member Variables for the TextViews
        private TextView mName;
        private TextView mAddress;
        private  ImageView mImageResource;

        /**
         * Constructor for the ViewHolder, used in onCreateViewHolder().
         *
         * @param itemView The rootview of the list_item.xml layout file.
         */
        ViewHolder(View itemView) {
            super(itemView);

            // Initialize the views.
            mName = (TextView) itemView.findViewById(R.id.theatre_name);
            mAddress = itemView.findViewById(R.id.address);
            mImageResource = itemView.findViewById(R.id.theatreImage);
            // Set the OnClickListener to the entire view.
            itemView.setOnClickListener(this);
        }

        void bindTo(Theatre current){
            // Populate the textviews with data.
            mName.setText(current.getName());
            mAddress.setText(current.getAddress());

            // Load the images into the ImageView using the Glide library.
            Glide.with(mContext).load(
                    current.getImageResource()).into(mImageResource);
        }

        /**
         * Handle click to show DetailActivity.
         *
         * @param view View that is clicked.
         */
        @Override
        public void onClick(View view) {
            Theatre current = mTheatreData.get(getAdapterPosition());
            Intent detailIntent = new Intent(mContext, DetailActivity.class);
            detailIntent.putExtra("name", current.getName());
            detailIntent.putExtra("image_resource",
                    current.getImageResource());
            detailIntent.putStringArrayListExtra("dates",current.getDate());
            mContext.startActivity(detailIntent);
        }
    }
}
