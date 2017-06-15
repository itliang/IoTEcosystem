package com.example.liliang.myapplication;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.IBinder;
import android.os.RemoteException;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import java.util.List;

//Service client
public class MainActivity extends AppCompatActivity {

    public final String TAG = this.getClass().getName();

    private BookManager bookManager = null;
    private boolean mBound = false;
    private List<Book> mBooks;

    private Button button = null;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        button = (Button) findViewById(R.id.add_book);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                addBook();
            }
        });

        Button startSvcBtn = (Button) findViewById(R.id.start_service);
        Button stopSvcBtn = (Button) findViewById(R.id.stop_service);
        startSvcBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                operateService(true);
            }
        });

        stopSvcBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                operateService(false);
            }
        });
    }

    private void operateService(Boolean start){
        Intent intent = new Intent(this, AIDLService.class);
        if(start){
            startService(intent);
        }else{
            stopService(intent);
        }
    }

    private void addBook(){
        Intent intent = new Intent(this, AIDLService.class);
        if(!mBound){
            bindService();
            Log.d(TAG, "Not bind service yet");
            return;
        }

        if(bookManager == null){
            return;
        }
        Book book = new Book();
        book.setName("aaaaaaaaaaa");
        book.setPrice(111111);
        try {
            bookManager.addBook(book);
            Log.d(TAG, book.toString());
        }catch (RemoteException e){
            e.printStackTrace();
        }
    }

    private void bindService(){
        Intent intent = new Intent(this, AIDLService.class);
        intent.setAction("");
        intent.setPackage("com.example.liliang.myapplication");
        bindService(intent, mServiceConnection, Context.BIND_AUTO_CREATE);
    }

    @Override
    protected void onStart() {
        super.onStart();
        bindService();

    }

    @Override
    protected void onStop() {
        super.onStop();
    }

    private ServiceConnection mServiceConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            bookManager = BookManager.Stub.asInterface(service);
            mBound = true;
            if(bookManager != null){
                try{
                    mBooks = bookManager.getBooks();
                    Log.d(TAG, mBooks.toString());
                }catch (RemoteException e){
                    e.printStackTrace();
                }

            }
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            Log.d(TAG, "Service disconnected");
            mBound = false;
        }
    };
}
