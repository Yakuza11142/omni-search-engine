package com.example.omnisys

import android.os.Bundle
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity

class MainActivity : AppCompatActivity() {

    init {
        // Loads libmemory_node.so compiled by your GitHub Actions workflow
        System.loadLibrary("memory_node")
    }

    // External JNI hook linking to your C function
    external fun getSystemStatus(): String

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val statusTextView = findViewById<TextView>(R.id.statusText)

        try {
            statusTextView.text = getSystemStatus()
        } catch (e: UnsatisfiedLinkError) {
            statusTextView.text = "Error connecting C library: ${e.message}"
        }
    }
}
