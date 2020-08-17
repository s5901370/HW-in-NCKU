package com.example.zerone.moviehelper;

import java.util.ArrayList;
import java.util.Iterator;

public class Theatre {
    private String name;
    private String address;
    private ArrayList<String> dates;
    private final int imageResource;
    Theatre(String name,String address,int imageResource,Iterator<String> dates){
        this.name = name;
        this.address = address;
        this.imageResource = imageResource;
        ArrayList<String> a= new ArrayList<>();
        while(dates.hasNext()){
            a.add(dates.next());
        }
        this.dates=a;
    }

    public String getName(){return name;}
    public String getAddress(){return address;}
    public ArrayList<String> getDate(){ return dates;}
    public int getImageResource() {
        return imageResource;
    }
}
