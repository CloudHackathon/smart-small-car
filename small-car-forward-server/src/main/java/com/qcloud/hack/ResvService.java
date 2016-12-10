package com.qcloud.hack;

import com.qcloud.hack.config.Constant;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;

/**
 * @author lvyahui (lvyahui8@gmail.com,lvyahui8@126.com)
 * @since 2016/12/10 12:56
 */
public class ResvService {

    private static Socket gestureClient;
    private static Socket songClient;
    private static Socket ctrlClient;


    public static void main(String[] args) {
        try {
            final ServerSocket gestureServer = new ServerSocket(Constant.port.gesture);
            final ServerSocket issuedServer = new ServerSocket(Constant.port.instruct);
            final ServerSocket songServer = new ServerSocket(Constant.port.song);

            new Thread(new Runnable() {
                public void run() {
                    while (true) {
                        try {
                            ResvService.gestureClient = gestureServer.accept();
                            System.out.println("gesture in");
                            forward();
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }

                }
            }).start();

            new Thread(new Runnable() {
                public void run() {
                    while (true) {
                        try {

                            ResvService.songClient = songServer.accept();
                            System.out.println("song in");
                            forward();
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }

                }
            }).start();

            new Thread(new Runnable() {
                public void run() {
                    while (true) {
                        try {

                            ResvService.ctrlClient = issuedServer.accept();
                            System.out.println("ctrl in");
                            forward();
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                }
            }).start();

//            gestureServer.close();
//            songServer.close();
//            issuedServer.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static void forward() {
        if (gestureClient != null && !gestureClient.isClosed()
                && songClient != null && !songClient.isClosed()
                && ctrlClient != null && !ctrlClient.isClosed()) {
            System.out.println("all in");
            try {
                // 手势指令录入
                InputStream gestureInput = gestureClient.getInputStream();
                // 小车指令和声音指令发出
                OutputStream pcOutput = ctrlClient.getOutputStream();
                OutputStream songOutput = songClient.getOutputStream();

                int inst;
                int last = -1;
                while ((inst = gestureInput.read()) != -1) {
                    inst = inst - 48;
                    System.out.println(inst);
                    if (inst > 4) {
                        // 不接受的指令
                        break;
                    }
                    // 发送声音
                    if(inst != last){
                        songOutput.write(Constant.songMap.get(inst).getBytes());
                        pcOutput.write(inst);
                        last = inst;
                    }
                }

                gestureClient.close();
                songClient.close();
                ctrlClient.close();
            } catch (Exception e) {
                System.out.println("链接断掉:"+e.getMessage());
                e.printStackTrace();
            }

        }
    }
}
