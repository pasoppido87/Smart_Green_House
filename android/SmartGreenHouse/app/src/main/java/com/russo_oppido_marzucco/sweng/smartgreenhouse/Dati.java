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
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.lang.reflect.Array;
import java.util.UUID;
import android.os.Handler;

public class Dati extends AppCompatActivity {


    int temperaturaVar,umiditaVar,luceVar,altezzaVar,umiditaTerraVar;

    TextView umiditaAria;
    TextView luceAria;
    TextView temperaturaAria;
    TextView umiditaTerreno;
    TextView altezza;

    public static String EXTRA_ADDRESS = "device_address";
    public static String IS_BT_CONNECT="device_connected";
    public String address = null;
    private ProgressDialog progress;
    BluetoothAdapter myBluetooth = null;
    public static BluetoothSocket btSocket = null;

    public static boolean isBtConnected = false;

    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");


    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_dati);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        final Context context=Dati.this;

        Intent newint = getIntent();
        address = newint.getStringExtra(DeviceList.EXTRA_ADDRESS);
        btSocket=ManualControl.btSocket;
        isBtConnected=ManualControl.isBtConnected;

        new ConnectBT().execute();
        final FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.syncFab);



        temperaturaAria= (TextView)findViewById(R.id.LetturaTemperatura);
        umiditaAria= (TextView)findViewById(R.id.LetturaUmidita);
        luceAria= (TextView)findViewById(R.id.LetturaLuce);
        altezza= (TextView)findViewById(R.id.altezza);
        umiditaTerreno= (TextView)findViewById(R.id.LetturaUmiditaTerra);


        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                receivedData();
               /*AlertDialog.Builder alertDialog = new AlertDialog.Builder(context);
                alertDialog.setTitle("Avviso");
                alertDialog.setMessage("Lettura dati completata!");
                alertDialog.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {

                    }
                });
                alertDialog.show();*/

            }
        });



    }


    private void receivedData()
    {

        char[] buffer = new char[1024];

        if (btSocket!=null)
        {
            String found="";
            String t1="";
            String t2="";
            String t3="";
            String t4="";
            String t5="";
            int j=0;
            int i;
                try {
                    BufferedReader in = new BufferedReader(new InputStreamReader(btSocket.getInputStream()));
                    in.read(buffer);

                    String string =new String(buffer);

                    try{
                        String[] valori= string.split("\n");
                        int length=  Array.getLength(valori);

                        for(i=0;i<length-1;i++){
                           if( valori[i].startsWith("*")){
                               found=valori[i];
                               j=i;
                           }
                        }

                        i=found.indexOf(" ");
                        t1=found.substring(1,i+1);
                        found=found.substring(i+1,found.length());
                        i=found.indexOf(" ");
                        t2=found.substring(0,i+1);
                        found=found.substring(i+1,found.length());
                        i=found.indexOf(" ");
                        t3=found.substring(0,i+1);
                        found=found.substring(i+1,found.length());
                        i=found.indexOf(" ");
                        t4=found.substring(0,i+1);
                        found=found.substring(i+1,found.length());
                        i=found.indexOf(" ");
                        t5=found.substring(0,i+1);

                        temperaturaAria.setText(t1);
                        umiditaAria.setText(t2);
                        luceAria.setText(t3);
                        altezza.setText(t4);
                        umiditaTerreno.setText(t5);


                        String[] temp=t1.split(" ");
                        temperaturaVar=Integer.parseInt(temp[0]);
                        temp=t2.split(" ");
                        umiditaVar= Integer.parseInt(temp[0]);
                        temp=t3.split(" ");
                        luceVar= Integer.parseInt(temp[0]);
                        temp=t4.split(" ");
                        altezzaVar= Integer.parseInt(temp[0]);
                        temp=t5.split(" ");
                        umiditaTerraVar= Integer.parseInt(temp[0]);
                        Toast.makeText(getApplicationContext(),"Dati aggiornati!",Toast.LENGTH_SHORT).show();

                    }catch (Exception e){
                        Toast.makeText(getApplicationContext(),"Errore sincronizzazione! Riprovare.",Toast.LENGTH_SHORT).show();

                    }

                    setAlertColor(temperaturaVar,umiditaVar,luceVar,altezzaVar,umiditaTerraVar);


                } catch (IOException e) {
                    msg("Errore!");
                }



        }
    }


    public void setAlertColor(int i1, int i2, int i3, int i4, int i5){
        if(i1>22 || i1 <18){
            temperaturaAria.setBackgroundColor(getResources().getColor(R.color.alert));

        }else {
            temperaturaAria.setBackgroundColor(getResources().getColor(R.color.colorAccent));
        }

        if(i2>75 || i2<45){
            umiditaAria.setBackgroundColor(getResources().getColor(R.color.alert));
        }else {
            umiditaAria.setBackgroundColor(getResources().getColor(R.color.colorAccent));
        }

        if(i3<40){
            luceAria.setBackgroundColor(getResources().getColor(R.color.alert));
        }else {
            luceAria.setBackgroundColor(getResources().getColor(R.color.colorAccent));
        }

        if(i4>20){
            altezza.setBackgroundColor(getResources().getColor(R.color.green));
        }else {
            altezza.setBackgroundColor(getResources().getColor(R.color.colorAccent));
        }

        if(i5<35 || i5>60){
            umiditaTerreno.setBackgroundColor(getResources().getColor(R.color.alert));
        }else {
            umiditaTerreno.setBackgroundColor(getResources().getColor(R.color.colorAccent));
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
        protected void onPreExecute() {

            {
                progress = ProgressDialog.show(Dati.this, "Connessione...", "Attendere prego!!!");
            }
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
                msg("Connesso.");
                isBtConnected = true;
            }
            progress.dismiss();
        }
    }



    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {

        int id = item.getItemId();

        switch (id){
            case R.id.manualControl: {
                Intent setting = new Intent(Dati.this, ManualControl.class);
                setting.putExtra(EXTRA_ADDRESS, address);
                setting.putExtra(IS_BT_CONNECT,isBtConnected);
                startActivity(setting);


                break;
            }


        }

        return super.onOptionsItemSelected(item);
    }







}
