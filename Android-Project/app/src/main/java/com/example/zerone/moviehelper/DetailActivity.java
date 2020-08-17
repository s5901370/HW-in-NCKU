package com.example.zerone.moviehelper;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.TextView;
import android.widget.Toast;

import com.bumptech.glide.Glide;

import java.util.ArrayList;
import java.util.Calendar;

public class DetailActivity extends AppCompatActivity {
    private RadioButton btn1;
    private RadioButton btn2;
    private RadioButton btn3;
    private RadioButton btn4;
    private RadioButton btn5;
    private RadioButton btn6;
    private RadioButton btn7;
    int month;
    int day ;
    int date ;
    private TextView mSession;//to store movie sessions
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_detail);
        ImageView Image = findViewById(R.id.theatreImage);
        TextView Name = findViewById(R.id.name);
        ArrayList<String> dates = new ArrayList<>();
        mSession = findViewById(R.id.session);

        Name.setText(getIntent().getStringExtra("name"));
        Glide.with(this).load(getIntent().getIntExtra("image_resource",0))
                .into(Image);
        RadioGroup rg = findViewById(R.id.group);
        RadioButton radioButton = new RadioButton(this);

        initial();



    }
    public void initial(){
        btn1 = findViewById(R.id.day1);
        btn2 = findViewById(R.id.day2);
        btn3 = findViewById(R.id.day3);
        btn4 = findViewById(R.id.day4);
        btn5 = findViewById(R.id.day5);
        btn6 = findViewById(R.id.day6);
        btn7 = findViewById(R.id.day7);

        Calendar calendar = Calendar.getInstance();
        month = calendar.get(Calendar.MONTH)+1;
         day = calendar.get(Calendar.DAY_OF_MONTH);
         date = calendar.get(Calendar.DAY_OF_WEEK);
        String d;
        switch (date){
            case Calendar.MONDAY:
                d = Integer.toString(month)+"/"+Integer.toString(day%30)+"(一)";
                btn1.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+1)%30)+"(二)";
                btn2.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+2)%30)+"(三)";
                btn3.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+3)%30)+"(四)";
                btn4.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+4)%30)+"(五)";
                btn5.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+5)%30)+"(六)";
                btn6.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+6)%30)+"(日)";
                btn7.setText(d);
                break;
            case Calendar.TUESDAY:
                d = Integer.toString(month)+"/"+Integer.toString(day%30)+"(二)";
                btn1.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+1)%30)+"(三)";
                btn2.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+2)%30)+"(四)";
                btn3.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+3)%30)+"(五)";
                btn4.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+4)%30)+"(六)";
                btn5.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+5)%30)+"(日)";
                btn6.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+6)%30)+"(一)";
                btn7.setText(d);
                break;
            case Calendar.WEDNESDAY:
                d = Integer.toString(month)+"/"+Integer.toString(day%30)+"(三)";
                btn1.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+1)%30)+"(四)";
                btn2.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+2)%30)+"(五)";
                btn3.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+3)%30)+"(六)";
                btn4.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+4)%30)+"(日)";
                btn5.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+5)%30)+"(一)";
                btn6.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+6)%30)+"(二)";
                btn7.setText(d);
                break;
            case Calendar.THURSDAY:
                d = Integer.toString(month)+"/"+Integer.toString(day%30)+"(四)";
                btn1.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+1)%30)+"(五)";
                btn2.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+2)%30)+"(六)";
                btn3.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+3)%30)+"(日)";
                btn4.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+4)%30)+"(一)";
                btn5.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+5)%30)+"(二)";
                btn6.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+6)%30)+"(三)";
                btn7.setText(d);
                break;
            case Calendar.FRIDAY:
                d = Integer.toString(month)+"/"+Integer.toString(day%30)+"(五)";
                btn1.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+1)%30)+"(六)";
                btn2.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+2)%30)+"(日)";
                btn3.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+3)%30)+"(一)";
                btn4.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+4)%30)+"(二)";
                btn5.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+5)%30)+"(三)";
                btn6.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+6)%30)+"(四)";
                btn7.setText(d);
                break;
            case Calendar.SATURDAY:
                d = Integer.toString(month)+"/"+Integer.toString(day%30)+"(六)";
                btn1.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+1)%30)+"(日)";
                btn2.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+2)%30)+"(一)";
                btn3.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+3)%30)+"(二)";
                btn4.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+4)%30)+"(三)";
                btn5.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+5)%30)+"(四)";
                btn6.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+6)%30)+"(五)";
                btn7.setText(d);
                break;
            case Calendar.SUNDAY:
                d = Integer.toString(month)+"/"+Integer.toString(day%30)+"日)";
                btn1.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+1)%30)+"(一)";
                btn2.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+2)%30)+"(二)";
                btn3.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+3)%30)+"(三)";
                btn4.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+4)%30)+"(四)";
                btn5.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+5)%30)+"(五)";
                btn6.setText(d);
                d = Integer.toString(month)+"/"+Integer.toString((day+6)%30)+"(六)";
                btn7.setText(d);
                break;
        }
    }

    public void onclickchanged(View view) {
        switch(view.getId()) {
            case R.id.day1:
                btn2.setChecked(false);
                btn3.setChecked(false);
                btn4.setChecked(false);
                btn5.setChecked(false);
                btn6.setChecked(false);
                btn7.setChecked(false);

                break;
            case R.id.day2:
                btn1.setChecked(false);
                btn3.setChecked(false);
                btn4.setChecked(false);
                btn5.setChecked(false);
                btn6.setChecked(false);
                btn7.setChecked(false);

                break;
            case R.id.day3:
                btn1.setChecked(false);
                btn2.setChecked(false);
                btn4.setChecked(false);
                btn5.setChecked(false);
                btn6.setChecked(false);
                btn7.setChecked(false);

                break;
            case R.id.day4:
                btn1.setChecked(false);
                btn2.setChecked(false);
                btn3.setChecked(false);
                btn5.setChecked(false);
                btn6.setChecked(false);
                btn7.setChecked(false);

                break;
            case R.id.day5:
                btn1.setChecked(false);
                btn2.setChecked(false);
                btn3.setChecked(false);
                btn4.setChecked(false);
                btn6.setChecked(false);
                btn7.setChecked(false);

                break;
            case R.id.day6:
                btn1.setChecked(false);
                btn2.setChecked(false);
                btn3.setChecked(false);
                btn4.setChecked(false);
                btn5.setChecked(false);
                btn7.setChecked(false);

                break;
            case R.id.day7:
                btn1.setChecked(false);
                btn2.setChecked(false);
                btn3.setChecked(false);
                btn4.setChecked(false);
                btn5.setChecked(false);
                btn6.setChecked(false);
                break;
            default:
                // Do nothing.
                break;
        }
    }


}
