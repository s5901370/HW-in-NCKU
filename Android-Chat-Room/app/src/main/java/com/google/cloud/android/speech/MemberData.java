package com.google.cloud.android.speech;

class MemberData {
    private String image;
    private String url;
    private String title;
    public MemberData(String image,String url,String title) {
        this.url = url;
        this.title = title;
        this.image = image;
    }

    // Add an empty constructor so we can later parse JSON into MemberData using Jackson
    public MemberData() {
    }

    public String getImage() {
        return image;
    }
public String getUrl(){return url;}
public String getTitle(){return title;}
}
