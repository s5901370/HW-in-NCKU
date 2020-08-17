package com.google.cloud.android.speech;

/*
這個類別定義了各個服務下面的關鍵字
 */
public abstract class SpeechReaction {
    public final String TAG = "SpeechReaction";

    //當使用者講完一句話之後會call這個方法,請在裡面自己寫想要的邏輯
    // text :辨識的結果
    public abstract void onSentenceEnd(String text);


}
