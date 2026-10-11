package com.example.omnisys

import android.os.Bundle
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity

class MainActivity : AppCompatActivity() {

    init {
        // Loads your compiled C shared library (.so)
        System.loadLibrary("memory_node")
    }

    // JNI hook linking directly to your C memory nexus function
    external fun getSystemStatus(): String

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val statusTextView = findViewById<TextView>(R.id.statusText)

        try {
            // Calls your C code and displays the exact memory pool results on screen
            val engineOutput = getSystemStatus()
            statusTextView.text = engineOutput
        } catch (e: UnsatisfiedLinkError) {
            statusTextView.text = "Error: Could not link C library (${e.message})"
        }
    }
}
