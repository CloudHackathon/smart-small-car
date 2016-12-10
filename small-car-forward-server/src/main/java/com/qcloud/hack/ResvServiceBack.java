package com.qcloud.hack;

import com.qcloud.hack.config.Constant;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * @author lvyahui (lvyahui8@gmail.com,lvyahui8@126.com)
 * @since 2016/12/10 12:56
 */
public class ResvServiceBack {

    private static Socket gestureClient;
    private static Socket songClient;
    private static Socket ctrlClient;


    public static void main(String[] args) throws IOException {
        final ServerSocket gestureServer = new ServerSocket(Constant.port.gesture);
        final ServerSocket issuedServer  = new ServerSocket(Constant.port.instruct);
        final ServerSocket songServer = new ServerSocket(Constant.port.song);

        ExecutorService threadPool = Executors.newCachedThreadPool();
        final CountDownLatch whistler = new CountDownLatch(1);
        final CountDownLatch tasks = new CountDownLatch(2);
        threadPool.submit(new Runnable() {
            public void run() {
                try {
                    whistler.await();
                    ResvServiceBack.gestureClient = gestureServer.accept();
                    System.out.println("gesture in");
                    tasks.countDown();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });

        threadPool.submit(new Runnable() {
            public void run() {
                try {
                    whistler.await();
                    ResvServiceBack.songClient = songServer.accept();
                    System.out.println("song in");
                    tasks.countDown();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });

       // threadPool.submit(new Runnable() {
       //     public void run() {
       //         try {
       //             whistler.await();
       //             ResvService.ctrlClient = issuedServer.accept();
       //             System.out.println("ctrl in");
       //             tasks.countDown();
       //         } catch (Exception e) {
       //             e.printStackTrace();
       //         }
       //     }
       // });

        whistler.countDown();
        try {
            tasks.await();
            threadPool.shutdown();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("all in");
        // 手势指令录入
        InputStream gestureInput = gestureClient.getInputStream();
        // 小车指令和声音指令发出
//        OutputStream pcOutput = ctrlClient.getOutputStream();
        OutputStream songOutput = songClient.getOutputStream();

        int inst ;
        while((inst = gestureInput.read()) != -1){
            System.out.println(inst);
            if(inst > 4){
                // 不接受的指令
                break;
            }
            // 发送声音
            songOutput.write(Constant.songMap.get(inst).getBytes());
//            pcOutput.write(inst);
        }

        gestureClient.close();
        songClient.close();
        //        ctrlClient.close();
        gestureServer.close();
        songServer.close();
        issuedServer.close();
    }
}
