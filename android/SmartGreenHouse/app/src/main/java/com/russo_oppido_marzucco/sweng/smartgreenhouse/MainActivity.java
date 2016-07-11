/*
Copyright 2016 Marzucco Angelo
Copyright 2016 Russo Andrea
Copyright 2016 Oppido Pasquale

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/


package com.russo_oppido_marzucco.sweng.smartgreenhouse;


import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import java.io.IOException;

public class MainActivity extends AppCompatActivity {
    public static Boolean isBtConnected = false;
    public static BluetoothSocket bluetoothSocket=null;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button start = (Button)findViewById(R.id.startBtn);

        start.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this, DeviceList.class);
                startActivity(intent);

            }
        });

    }



    @Override
    protected void onDestroy() {
        super.onDestroy();
        bluetoothSocket=Dati.btSocket;
        isBtConnected=Dati.isBtConnected;
        if(bluetoothSocket!=null && isBtConnected)
        {
            try {
                isBtConnected=false;
                bluetoothSocket.close();
            }catch (IOException ioe){
                Toast.makeText(getApplicationContext(),"Errore disconnessione bluetooth!",Toast.LENGTH_SHORT);
            }
            finish();

        }
    }
}

