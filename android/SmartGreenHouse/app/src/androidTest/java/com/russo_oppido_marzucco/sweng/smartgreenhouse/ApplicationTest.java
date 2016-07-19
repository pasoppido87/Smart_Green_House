package com.russo_oppido_marzucco.sweng.smartgreenhouse;

import android.app.Activity;
import android.app.Application;
import android.content.Intent;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.test.ActivityInstrumentationTestCase2;
import android.test.ApplicationTestCase;
import android.text.style.BackgroundColorSpan;
import android.widget.TextView;

import org.hamcrest.Matcher;

import static android.support.test.espresso.Espresso.*;
import static android.support.test.espresso.action.ViewActions.*;
import static android.support.test.espresso.assertion.ViewAssertions.matches;
import static android.support.test.espresso.matcher.ViewMatchers.*;
import static org.hamcrest.Matchers.equalTo;
import static org.hamcrest.Matchers.hasProperty;
import static org.hamcrest.Matchers.hasToString;
import static org.hamcrest.Matchers.startsWith;

/**
 * <a href="http://d.android.com/tools/testing/testing_android.html">Testing Fundamentals</a>*/

public class ApplicationTest extends ActivityInstrumentationTestCase2<MainActivity> {

         public ApplicationTest() {
                 super(MainActivity.class);
             }

    @Override
    protected void setUp() throws Exception {
        super.setUp();
        getActivity();
    }



    public void testOk() throws InterruptedException {
        /*Temperatura 20°C
        * Umidità 50%
        * Altezza 15cm
        * Umidità terreno 50%
        * Luminosità 50%*/
        String pkg="com.russo_oppido_marzucco.sweng.smartgreenhouse";
        onView(withId(R.id.startBtn)).perform(click());
        onView(withId(R.id.button)).perform(click());
        onData((hasToString(startsWith("HC-05")))).perform(click());
        onView(withId(R.id.syncFab)).perform(click());
        onView(withId(R.id.LetturaTemperatura)).check(matches(withText("20 ")));
        onView(withId(R.id.LetturaUmidita)).check(matches(withText("50 ")));
        onView(withId(R.id.LetturaLuce)).check(matches(withText("50 ")));
        onView(withId(R.id.altezza)).check(matches(withText("15 ")));
        onView(withId(R.id.LetturaUmiditaTerra)).check(matches(withText("50 ")));
        Thread.sleep(1000);

    }

    public void testTemp15() throws InterruptedException {
        String pkg="com.russo_oppido_marzucco.sweng.smartgreenhouse";
        onView(withId(R.id.startBtn)).perform(click());
        onView(withId(R.id.button)).perform(click());
        onData((hasToString(startsWith("HC-05")))).perform(click());
        onView(withId(R.id.syncFab)).perform(click());
        onView(withId(R.id.LetturaTemperatura)).check(matches(withText("15 ")));
        Thread.sleep(1000);
    }

    public void testTemp25() throws InterruptedException {
        String pkg="com.russo_oppido_marzucco.sweng.smartgreenhouse";
        onView(withId(R.id.startBtn)).perform(click());
        onView(withId(R.id.button)).perform(click());
        onData((hasToString(startsWith("HC-05")))).perform(click());
        onView(withId(R.id.syncFab)).perform(click());
        onView(withId(R.id.LetturaTemperatura)).check(matches(withText("25 ")));
        Thread.sleep(1000);
    }

    public void testUmidita80() throws InterruptedException {
        String pkg="com.russo_oppido_marzucco.sweng.smartgreenhouse";
        onView(withId(R.id.startBtn)).perform(click());
        onView(withId(R.id.button)).perform(click());
        onData((hasToString(startsWith("HC-05")))).perform(click());
        onView(withId(R.id.syncFab)).perform(click());
        onView(withId(R.id.LetturaUmidita)).check(matches(withText("80 ")));
        Thread.sleep(1000);
    }


    public void testUmidita40() throws InterruptedException {
        String pkg="com.russo_oppido_marzucco.sweng.smartgreenhouse";
        onView(withId(R.id.startBtn)).perform(click());
        onView(withId(R.id.button)).perform(click());
        onData((hasToString(startsWith("HC-05")))).perform(click());
        onView(withId(R.id.syncFab)).perform(click());
        onView(withId(R.id.LetturaUmidita)).check(matches(withText("40 ")));
        Thread.sleep(1000);
    }

    public void testLuce35() throws InterruptedException {
        String pkg="com.russo_oppido_marzucco.sweng.smartgreenhouse";
        onView(withId(R.id.startBtn)).perform(click());
        onView(withId(R.id.button)).perform(click());
        onData((hasToString(startsWith("HC-05")))).perform(click());
        onView(withId(R.id.syncFab)).perform(click());
        onView(withId(R.id.LetturaLuce)).check(matches(withText("35 ")));
        Thread.sleep(1000);
    }

    public void testAltezza22() throws InterruptedException {
        String pkg="com.russo_oppido_marzucco.sweng.smartgreenhouse";
        onView(withId(R.id.startBtn)).perform(click());
        onView(withId(R.id.button)).perform(click());
        onData((hasToString(startsWith("HC-05")))).perform(click());
        onView(withId(R.id.syncFab)).perform(click());
        onView(withId(R.id.altezza)).check(matches(withText("22 ")));
        Thread.sleep(1000);
    }

    public void testAllAlert() throws InterruptedException {

        /*Temperatura 15°C
        * Umidità 30%
        * Altezza 25cm
        * Umidità terreno 20%
        * Luminosità 30%*/

        String pkg="com.russo_oppido_marzucco.sweng.smartgreenhouse";
        onView(withId(R.id.startBtn)).perform(click());
        onView(withId(R.id.button)).perform(click());
        onData((hasToString(startsWith("HC-05")))).perform(click());
        onView(withId(R.id.syncFab)).perform(click());
        onView(withId(R.id.LetturaTemperatura)).check(matches(withText("15 ")));
        onView(withId(R.id.LetturaUmidita)).check(matches(withText("30 ")));
        onView(withId(R.id.LetturaLuce)).check(matches(withText("30 ")));
        onView(withId(R.id.altezza)).check(matches(withText("25 ")));
        onView(withId(R.id.LetturaUmiditaTerra)).check(matches(withText("20 ")));
        Thread.sleep(1000);
    }


    public void testManualSwitch() throws InterruptedException {

        /*Temperatura 15°C
        * Umidità 30%
        * Altezza 25cm
        * Umidità terreno 20%
        * Luminosità 30%*/

        String pkg="com.russo_oppido_marzucco.sweng.smartgreenhouse";
        onView(withId(R.id.startBtn)).perform(click());
        onView(withId(R.id.button)).perform(click());
        onData((hasToString(startsWith("HC-05")))).perform(click());
        onView(withId(R.id.manualControl)).perform(click());
        onView(withId(R.id.manualSwitch)).perform(click()).check(matches(isChecked()));
        Thread.sleep(1000);
        onView(withId(R.id.manualSwitch)).perform(click()).check(matches(isNotChecked()));
        Thread.sleep(1000);

    }

    public void testAllManualSwitch() throws InterruptedException {

        /*Temperatura 15°C
        * Umidità 30%
        * Altezza 25cm
        * Umidità terreno 20%
        * Luminosità 30%*/

        String pkg="com.russo_oppido_marzucco.sweng.smartgreenhouse";
        onView(withId(R.id.startBtn)).perform(click());
        onView(withId(R.id.button)).perform(click());
        onData((hasToString(startsWith("HC-05")))).perform(click());
        onView(withId(R.id.manualControl)).perform(click());
        onView(withId(R.id.manualSwitch)).perform(click()).check(matches(isChecked()));
        Thread.sleep(1000);
        onView(withId(R.id.luceSwitch)).perform(click()).check(matches(isChecked()));
        Thread.sleep(1000);
        onView(withId(R.id.VentolaSwitch)).perform(click()).check(matches(isChecked()));
        Thread.sleep(1000);
        onView(withId(R.id.irrigazioneSwitch)).perform(click()).check(matches(isChecked()));
        Thread.sleep(1000);
        onView(withId(R.id.riscaldamentoSwitch)).perform(click()).check(matches(isChecked()));
        Thread.sleep(1000);
        onView(withId(R.id.luceSwitch)).perform(click()).check(matches(isNotChecked()));
        Thread.sleep(1000);
        onView(withId(R.id.VentolaSwitch)).perform(click()).check(matches(isNotChecked()));
        Thread.sleep(1000);
        onView(withId(R.id.irrigazioneSwitch)).perform(click()).check(matches(isNotChecked()));
        Thread.sleep(1000);
        onView(withId(R.id.riscaldamentoSwitch)).perform(click()).check(matches(isNotChecked()));
        Thread.sleep(1000);
        onView(withId(R.id.manualSwitch)).perform(click()).check(matches(isNotChecked()));
        Thread.sleep(1000);

    }

    public void testAllOnManualOff() throws InterruptedException {

        /*Temperatura 15°C
        * Umidità 30%
        * Altezza 25cm
        * Umidità terreno 20%
        * Luminosità 30%*/

        String pkg="com.russo_oppido_marzucco.sweng.smartgreenhouse";
        onView(withId(R.id.startBtn)).perform(click());
        onView(withId(R.id.button)).perform(click());
        onData((hasToString(startsWith("HC-05")))).perform(click());
        onView(withId(R.id.manualControl)).perform(click());
        onView(withId(R.id.manualSwitch)).perform(click()).check(matches(isChecked()));
        Thread.sleep(1000);
        onView(withId(R.id.luceSwitch)).perform(click()).check(matches(isChecked()));
        Thread.sleep(1000);
        onView(withId(R.id.VentolaSwitch)).perform(click()).check(matches(isChecked()));
        Thread.sleep(1000);
        onView(withId(R.id.irrigazioneSwitch)).perform(click()).check(matches(isChecked()));
        Thread.sleep(1000);
        onView(withId(R.id.riscaldamentoSwitch)).perform(click()).check(matches(isChecked()));
        Thread.sleep(1000);
        onView(withId(R.id.manualSwitch)).perform(click()).check(matches(isNotChecked()));
        Thread.sleep(1000);

    }



     }
