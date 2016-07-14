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

import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.widget.CompoundButton;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.Switch;
import android.widget.Toast;
import java.io.IOException;
import java.util.UUID;

public class ManualControl extends AppCompatActivity {

public boolean manual=false;
    String address = null;
    private ProgressDialog progress;
    BluetoothAdapter myBluetooth = null;
    public static BluetoothSocket btSocket = null;
    public  static boolean isBtConnected = false;
    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Intent newint = getIntent();
        address = newint.getStringExtra(Dati.EXTRA_ADDRESS);
        isBtConnected=Dati.isBtConnected;
        btSocket=Dati.btSocket;

        setContentView(R.layout.activity_manual_control);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

            new ConnectBT().execute();

        Switch manualSwitch = (Switch)findViewById(R.id.manualSwitch);
        final Switch luceSwitch = (Switch)findViewById(R.id.luceSwitch);
        final Switch ventolaSwitch=(Switch)findViewById(R.id.VentolaSwitch);
        final Switch irrigazioneSwitch = (Switch) findViewById(R.id.irrigazioneSwitch);
        final Switch riscaldamentoSwitch = (Switch)findViewById(R.id.riscaldamentoSwitch);

        manualSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {

                if(isChecked){
                    sendData(1);
                    setManualDisplay(isChecked);
                }
                else{
                   luceSwitch.setChecked(false);
                    ventolaSwitch.setChecked(false);
                    irrigazioneSwitch.setChecked(false);
                    riscaldamentoSwitch.setChecked(false);
                    setManualDisplay(isChecked);
                    sendData(0);
                }

            }
        });


        luceSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked){
                    sendData(3);
                    ImageView imageView = (ImageView)findViewById(R.id.luceImg);
                    imageView.setImageResource(R.mipmap.luce_on);
                }
                else if(!isChecked){

                    sendData(2);
                    ImageView imageView = (ImageView)findViewById(R.id.luceImg);
                    imageView.setImageResource(R.mipmap.luce_off);
                }
            }
        });


       ventolaSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked){
                   sendData(5);
                    ImageView imageView = (ImageView)findViewById(R.id.ventolaImg);
                    imageView.setImageResource(R.mipmap.ventola_on);
                }
                else if(!isChecked){
                    sendData(4);
                    ImageView imageView = (ImageView)findViewById(R.id.ventolaImg);
                    imageView.setImageResource(R.mipmap.ventola_off);
                }
            }
        });

        irrigazioneSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked){
                    sendData(7);
                    ImageView imageView = (ImageView)findViewById(R.id.irrigazioneImg);
                    imageView.setImageResource(R.mipmap.irrigazione_on);

                }
                else if(!isChecked){
                    sendData(6);
                    ImageView imageView = (ImageView)findViewById(R.id.irrigazioneImg);
                    imageView.setImageResource(R.mipmap.irrigazione_off);

                }
            }
        });

        riscaldamentoSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked){
                    sendData(9);
                    ImageView imageView = (ImageView)findViewById(R.id.riscaldamentoImg);
                    imageView.setImageResource(R.mipmap.riscaldamento_on);

                }
                else if(!isChecked){
                    sendData(8);
                    ImageView imageView = (ImageView)findViewById(R.id.riscaldamentoImg);
                    imageView.setImageResource(R.mipmap.riscaldamento_off);

                }
            }
        });

    }

    @Override
    protected void onStop() {
        super.onStop();
        sendData(0);
    }

    public void setManualDisplay(boolean isManual ){
        LinearLayout luceLayout=(LinearLayout) findViewById(R.id.luceLayout);
        LinearLayout ventilaLayout=(LinearLayout) findViewById(R.id.ventolaLayout);
        LinearLayout irrigazioneLayout=(LinearLayout) findViewById(R.id.irrigazioneLayout);
        LinearLayout riscaldamentoLayout=(LinearLayout) findViewById(R.id.riscaldamentoLayout);
        if(isManual){
            luceLayout.setVisibility(View.VISIBLE);
            ventilaLayout.setVisibility(View.VISIBLE);
            irrigazioneLayout.setVisibility(View.VISIBLE);
            riscaldamentoLayout.setVisibility(View.VISIBLE);
            this.manual=true;
        }else{
            luceLayout.setVisibility(View.GONE);
            ventilaLayout.setVisibility(View.GONE);
            irrigazioneLayout.setVisibility(View.GONE);
            riscaldamentoLayout.setVisibility(View.GONE);
            this.manual=false;
        }
    }




    private void sendData(int i1)
    {
        if (btSocket!=null)
        {
            try
            {
                btSocket.getOutputStream().write(Integer.toString(i1).getBytes());
            }
            catch (IOException e)
            {
                msg("Errore!");
            }
        }
    }

    private void msg(String s)
    {
        Toast.makeText(getApplicationContext(),s,Toast.LENGTH_LONG).show();
    }


    private class ConnectBT extends AsyncTask<Void, Void, Void>
    {
        private boolean ConnectSuccess = true;

        @Override
        protected void onPreExecute()
        {
                progress = ProgressDialog.show(ManualControl.this, "In connessione...", "Attendere prego!!!");

        }

        @Override
        protected Void doInBackground(Void... devices)
        {
            try
            {
                if ((btSocket == null || !isBtConnected))
                {
                    myBluetooth = BluetoothAdapter.getDefaultAdapter();
                    BluetoothDevice dispositivo = myBluetooth.getRemoteDevice(address);
                    btSocket = dispositivo.createInsecureRfcommSocketToServiceRecord(myUUID);
                    BluetoothAdapter.getDefaultAdapter().cancelDiscovery();
                    btSocket.connect();
                }
            }
            catch (IOException e)
            {
                ConnectSuccess = false;
            }
            return null;
        }
        @Override
        protected void onPostExecute(Void result)
        {
            super.onPostExecute(result);

            if (!ConnectSuccess)
            {
                msg("Connessione fallita! Prova di nuovo.");
                finish();
            }
            else
            {
                msg("Connesso!");
                isBtConnected = true;
            }
            progress.dismiss();
        }
    }


}
