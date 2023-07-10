package com.bacar.trackat_android;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.widget.TextView;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;


public class MainActivity extends AppCompatActivity {

    private Handler handler;
    private TextView locationsTextView;
    private String locationsString = "";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        handler = new Handler(Looper.getMainLooper());
        handler.post(periodicRequest);


        locationsTextView = findViewById(R.id.location);


    }

    private Runnable periodicRequest = new Runnable() {
        @Override
        public void run() {
            sendGetRequest();
            handler.postDelayed(this, 30000);
        }
    };

    /**
     * Get from the server all the locations
     */
    private void sendGetRequest(){
        OkHttpClient client = new OkHttpClient();


        String loc_url = "https://242b-2607-fa49-ad41-de00-b4ae-2131-7496-2e12.ngrok-free.app/locations";

        Request request = new Request.Builder()
                .url(loc_url)
                .build();
        client.newCall(request).enqueue(new Callback() {
            @Override
            public void onFailure(@NonNull Call call, @NonNull IOException e) {
                locationsTextView.setText("Failed to load Locations");
            }

            @Override
            public void onResponse(@NonNull Call call, @NonNull Response response) throws IOException {
                if(response.isSuccessful()){
                    String locationResponse = response.body().string();
                    locationsString = "";
                    try {
                        JSONObject locations = new JSONObject(locationResponse);
                        for (int i = locations.length(); i > 0; i--){
                            JSONObject location = locations.getJSONObject(String.valueOf(i));
                            String dateStr = location.getString("date");
                            SimpleDateFormat sdf = new SimpleDateFormat("EEE, dd MMM yyyy HH:mm:ss z", Locale.ENGLISH);
                            Date date = sdf.parse(dateStr);
                            double latitude = location.getDouble("latitude");
                            double longitude = location.getDouble("longitude");

                            if (i == locations.length()){
                                locationsString = "Current Location\n\n" + latitude + ", " + longitude +"\n\nPast locations:\n";
                            }

                            locationsString += latitude+", "+longitude+" @ "
                                    +String.format(Locale.ENGLISH, "%02d", date.getHours())+":"
                                    +String.format(Locale.ENGLISH, "%02d", date.getMinutes())+":"
                                    +String.format(Locale.ENGLISH, "%02d", date.getSeconds())
                                    +" " + String.format(Locale.ENGLISH, "%02d", date.getDate())+"/"
                                    +String.format(Locale.ENGLISH, "%02d", date.getMonth()+1)+"/"
                                    +(date.getYear()+1900)
                                    +"\n";

                        }

                        runOnUiThread(() -> {
                            locationsTextView.setText(locationsString);
                        });

                    } catch (JSONException e) {
                        throw new RuntimeException(e);
                    }
                    catch (ParseException e) {
                        throw new RuntimeException(e);
                    }

                }
            }
        });
    }
}