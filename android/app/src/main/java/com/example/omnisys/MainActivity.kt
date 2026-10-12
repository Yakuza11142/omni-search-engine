package com.example.omnisys

import android.os.Bundle
import android.widget.Button
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity

class MainActivity : AppCompatActivity() {

    init {
        // Loads the compiled C shared library containing all your root .c files
        System.loadLibrary("memory_node")
    }

    // JNI hook mapping directly to your C multi-node aggregation function
    external fun getSystemStatus(): String

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val statusTextView = findViewById<TextView>(R.id.statusText)
        val refreshButton = findViewById<Button>(R.id.refreshButton)

        fun fetchEngineData() {
            try {
                // Pulls live data straight from your C backend nodes
                val engineOutput = getSystemStatus()
                statusTextView.text = engineOutput
            } catch (e: UnsatisfiedLinkError) {
                statusTextView.text = "Link Error: Could not connect to C core (${e.message})"
            }
        }

        // Fetch data automatically on launch
        fetchEngineData()

        // Re-sync on button tap
        refreshButton.setOnClickListener {
            fetchEngineData()
        }
    }
}
