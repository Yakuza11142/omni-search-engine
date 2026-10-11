package com.example.omnisys

import android.os.Bundle
import android.widget.Button
import android.widget.EditText
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity

class MainActivity : AppCompatActivity() {

    init {
        System.loadLibrary("memory_node")
    }

    // JNI hook to send user prompt to C and get a response
    external fun processCommand(input: String): String

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val chatLog = findViewById<TextView>(R.id.chatLog)
        val userInput = findViewById<EditText>(R.id.userInput)
        val sendButton = findViewById<Button>(R.id.sendButton)

        sendButton.setOnClickListener {
            val query = userInput.text.toString()
            if (query.isNotEmpty()) {
                // Append user message to chat log
                chatLog.append("\nUser: $query\n")
                userInput.text.clear()

                try {
                    // Send prompt to C backend and get response
                    val response = processCommand(query)
                    chatLog.append("Omni Engine: $response\n")
                } catch (e: UnsatisfiedLinkError) {
                    chatLog.append("Omni Engine: Error linking native library.\n")
                }
            }
        }
    }
}
