package com.qcloud.hack;

import com.qcloud.hack.config.Constant;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.Socket;
import java.net.URL;
import java.util.ArrayDeque;
import java.util.Arrays;
import java.util.Queue;

/**
 * @author lvyahui (lvyahui8@gmail.com,lvyahui8@126.com)
 * @since 2016/12/10 14:16
 */
public class VideoLive {

    public static final int WIDTH = 400;
    public static final int HEIGHT = 400;
    private static JFrame buildFrame() {
        JFrame frame = new JFrame();
        frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        frame.setSize(WIDTH, HEIGHT);
        frame.setVisible(true);
        return frame;
    }

    public static void main(String[] args) {

        int bufSize = 512 * 1024; // 视频图片缓冲
        byte[] jpg_buf = new byte[bufSize]; // buffer to read jpg

        int readSize = 4096; // 每次最大获取的流
        byte[] buffer = new byte[readSize]; // buffer to read stream
        String urlstr = "http://192.168.1.1:8080/?action=stream";
        JFrame jframe = buildFrame();
        ImagePanel jpanel = new ImagePanel();

        jframe.add(jpanel);

       // RtmpClient rtmpClient = new DefaultRtmpClient("rtmp://5594.livepush.myqcloud.com/live/5594_358d9f6f49?bizid=5594&txSecret=7bf9b77cfa6868288f9f5046f94383d2&txTime=584D77FF");
       // try {
       //     rtmpClient.connect();
       // } catch (IOException e) {
       //     e.printStackTrace();
       // }
        final Queue<byte[]> jpgs = new ArrayDeque<byte[]>();
        while (true) {
            long time ;
            long span;

            URL url;
            HttpURLConnection urlConn = null;

            try {
                url = new URL(urlstr);
                urlConn = (HttpURLConnection) url.openConnection(); // 使用HTTPURLConnetion打开连接

                time = System.currentTimeMillis();

                int read = 0;
                int status = 0;
                int jpg_count = 0; // jpg数据下标
                int fps = 0;
                int count = 0;

                while (true) {
                    read = urlConn.getInputStream().read(buffer, 0,
                            readSize);

                    if (read > 0) {

                        for (int i = 0; i < read; i++) {
                            switch (status) {
                                // Content-Length:
                                case 0:
                                    if (buffer[i] == (byte) 'C')
                                        status++;
                                    else
                                        status = 0;
                                    break;
                                case 1:
                                    if (buffer[i] == (byte) 'o')
                                        status++;
                                    else
                                        status = 0;
                                    break;
                                case 2:
                                    if (buffer[i] == (byte) 'n')
                                        status++;
                                    else
                                        status = 0;
                                    break;
                                case 3:
                                    if (buffer[i] == (byte) 't')
                                        status++;
                                    else
                                        status = 0;
                                    break;
                                case 4:
                                    if (buffer[i] == (byte) 'e')
                                        status++;
                                    else
                                        status = 0;
                                    break;
                                case 5:
                                    if (buffer[i] == (byte) 'n')
                                        status++;
                                    else
                                        status = 0;
                                    break;
                                case 6:
                                    if (buffer[i] == (byte) 't')
                                        status++;
                                    else
                                        status = 0;
                                    break;
                                case 7:
                                    if (buffer[i] == (byte) '-')
                                        status++;
                                    else
                                        status = 0;
                                    break;
                                case 8:
                                    if (buffer[i] == (byte) 'L')
                                        status++;
                                    else
                                        status = 0;
                                    break;
                                case 9:
                                    if (buffer[i] == (byte) 'e')
                                        status++;
                                    else
                                        status = 0;
                                    break;
                                case 10:
                                    if (buffer[i] == (byte) 'n')
                                        status++;
                                    else
                                        status = 0;
                                    break;
                                case 11:
                                    if (buffer[i] == (byte) 'g')
                                        status++;
                                    else
                                        status = 0;
                                    break;
                                case 12:
                                    if (buffer[i] == (byte) 't')
                                        status++;
                                    else
                                        status = 0;
                                    break;
                                case 13:
                                    if (buffer[i] == (byte) 'h')
                                        status++;
                                    else
                                        status = 0;
                                    break;
                                case 14:
                                    if (buffer[i] == (byte) ':')
                                        status++;
                                    else
                                        status = 0;
                                    break;
                                case 15:
                                    if (buffer[i] == (byte) 0xFF)
                                        status++;
                                    jpg_count = 0;
                                    jpg_buf[jpg_count++] = buffer[i];
                                    break;
                                case 16:
                                    if (buffer[i] == (byte) 0xD8) {
                                        status++;
                                        jpg_buf[jpg_count++] = buffer[i];
                                    } else {
                                        if (buffer[i] != (byte) 0xFF)
                                            status = 15;

                                    }
                                    break;
                                case 17:
                                    jpg_buf[jpg_count++] = buffer[i];
                                    if (buffer[i] == (byte) 0xFF)
                                        status++;
                                    if (jpg_count >= bufSize)
                                        status = 0;
                                    break;
                                case 18:
                                    jpg_buf[jpg_count++] = buffer[i];
                                    if (buffer[i] == (byte) 0xD9) {
                                        status = 0;
                                        // jpg接收完成

                                        fps++;
                                        span = System.currentTimeMillis()
                                                - time;
                                        if (span > 1000L) {
                                            time = System.currentTimeMillis();
                                            fps = 0;
                                        }
                                        // 显示图像
                                        {
                                            count++;
                                            BufferedImage image = ImageIO.read(new ByteArrayInputStream(jpg_buf));
                                            jpanel.setImage(image);
                                            //String file = "D:/tmp/video_in/"+ count + ".jpg";
                                            //OutputStream out = new FileOutputStream(file);
                                            //out.write(jpg_buf);
                                            //out.flush();
                                            //rtmpClient.play(file,null);
                                            if(jpgs.size() < 10000){
                                                jpgs.add(Arrays.copyOf(jpg_buf,jpg_buf.length));
                                            }

                                        }
                                    } else {
                                        if (buffer[i] != (byte) 0xFF)
                                            status = 17;
                                    }
                                    break;
                                default:
                                    status = 0;
                                    break;

                            }
                        }
                    }
                }
            } catch (IOException ex) {
                assert urlConn != null;
                urlConn.disconnect();
                ex.printStackTrace();
            }


            new Thread(new Runnable() {
                public void run() {
                    try{
                        Socket videoOut;
                        OutputStream videoOutStream;
                        try {
                            videoOut = new Socket(Constant.host.server,Constant.port.video_in);
                            videoOutStream = videoOut.getOutputStream();
                            System.out.println("connected video server");
                            if(videoOutStream != null){
                                while(true){
                                    if(jpgs.size() <= 0){
                                        Thread.sleep(1000);
                                    }
                                    byte [] jpg_buf = jpgs.poll();
                                    try {
                                        System.out.println("send");
                                        videoOutStream.write(jpg_buf);
                                        videoOutStream.flush();
                                    } catch (Exception e){

                                    }
                                }
                            }
                        } catch (IOException e) {
                            e.printStackTrace();
                        }

                    } catch (Exception e){

                    }
                }
            }).start();
        }

    }
}
