package com.qcloud.hack.client;

import com.qcloud.hack.config.Constant;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;

/**
 * Hello world!
 *
 */
public class CtrlApp
{
    private static final String CAR_SERVER_CTRL_IP = "192.168.1.1";
    private static final int CAR_SERVER_CTRL_PORT = 2001;

    private static final String INST_SERVER_IP = Constant.host.server;

    private static final int INST_SERVER_PORT = Constant.port.instruct;


    public static void main(String[] args ) throws IOException, InterruptedException {
        Socket carServer = new Socket(CAR_SERVER_CTRL_IP,CAR_SERVER_CTRL_PORT);
        System.out.println("connected car server");
        Socket instProducer = new Socket(INST_SERVER_IP,INST_SERVER_PORT);
        System.out.println("connected inst server");
        InputStream instIn = instProducer.getInputStream();
        OutputStream instSender = carServer.getOutputStream();
        try{
            int inst ;
            while((inst = instIn.read()) != -1){
                if(inst > 4){
                    // 不接受的指令
                    break;
                }
                System.out.println(inst);
                instSender.write(Constant.instructMap.get(inst));
                instSender.flush();
            }

        } finally {
            instSender.close();
            carServer.close();
            instIn.close();
            instProducer.close();
        }
    }
}
