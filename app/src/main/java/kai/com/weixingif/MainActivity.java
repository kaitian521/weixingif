package kai.com.weixingif;

import android.content.ComponentName;
import android.content.Intent;
import android.graphics.Movie;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;

import com.basv.gifmoviewview.widget.GifMovieView;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;

import kai.com.weixingif.gifsicle.GifSicleRequest;


public class MainActivity extends ActionBarActivity {

    static {
        System.loadLibrary("GifProcess");
    }

    Button button, button2;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        ComponentName co = getIntent().getComponent();
        Log.i("MainActivity", co.getPackageName() + " " + co.getClassName());

        String path = "/storage/sdcard0/gif";
        String name = "jiang.gif";

        setContentView(R.layout.activity_main);

        button = (Button) findViewById(R.id.button);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                ComponentName comp = new ComponentName(MainActivity.this, MainActivity.class);
                Intent intent = new Intent();
                intent.setComponent(comp);
                startActivity(intent);
            }
        });

        button2 = (Button) findViewById(R.id.button2);
        button2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
//                Intent intent = new Intent();
//                intent.setAction("android.intent.action.XXXXXXXX");
//                intent.addCategory("FUCKU");
//                startActivity(intent);
                send();
            }
        });

        final GifMovieView gif1 = (GifMovieView) findViewById(R.id.gif1);

        gif1.setMovie(Movie.decodeFile(path + "/" + name));
//        Movie movie = Movie.decodeFile(path + "/" + name);
//        Log.i("MainActivity", movie.height() + " " + movie.width());
    }

    private void send() {
        String ret = GifSicleRequest.getGifInfo("/storage/sdcard0/gif", "code.gif");
        String[] rett = ret.split(";");
        for (String str: rett) {
            Log.i("MainActivity", str);
        }
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    public void onGifClick(View v) {
        GifMovieView gif = (GifMovieView) v;
        gif.setPaused(!gif.isPaused());
    }

}
