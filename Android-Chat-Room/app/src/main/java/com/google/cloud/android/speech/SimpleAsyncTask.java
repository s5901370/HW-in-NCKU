package com.google.cloud.android.speech;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.AsyncTask;
import android.widget.ProgressBar;
import android.widget.TextView;

import java.lang.ref.WeakReference;
import java.util.Random;

import static android.app.Activity.RESULT_OK;

/**
 * The SimpleAsyncTask class extends AsyncTask to perform a very simple
 * background task -- in this case, it just sleeps for a random amount of time.
 */

public class SimpleAsyncTask extends AsyncTask<Void, Integer, Void> {

    // The TextView where we will show results
    private ProgressBar mBar;
    Activity mActivity;
    // Constructor that provides a reference to the TextView from the MainActivity
    SimpleAsyncTask(Activity activity, ProgressBar bar) {
        mBar = bar;
        mActivity = activity;
    }

    /**
     * Runs on the background thread.
     *
     * @param voids No parameters in this use case.
     * @return Returns the string including the amount of time that
     * the background thread slept.
     */
    @Override
    protected Void doInBackground(Void... voids) {

        mBar.setProgress(0);
        // Make the task take long enough that we have
        // time to rotate the phone while it is running.
        int s =100;
        int count = 1;
        // Sleep for the random amount of time.
        while(count<=50){
            try {
                Thread.sleep(s);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            publishProgress(Integer.valueOf(count));
            count++;
        }


        // Return a String result.
        return null;
    }

    @Override
    protected void onPostExecute(Void aVoid) {
        mActivity.finish();
    }

    /**
     * Does something with the result on the UI thread; in this case
     * updates the TextView.
     */


    @Override
    protected void onProgressUpdate(Integer... values) {
        mBar.setProgress(values[0]);

    }
}