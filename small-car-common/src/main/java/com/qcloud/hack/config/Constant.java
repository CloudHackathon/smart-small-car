package com.qcloud.hack.config;

import java.util.HashMap;
import java.util.Map;

/**
 * @author lvyahui (lvyahui8@gmail.com,lvyahui8@126.com)
 * @since 2016/12/10 13:56
 */
public class Constant {
    public final static class instruct{
        public final static byte [] forward = new byte[]{(byte) 0xff, (byte) 0x00, (byte) 0x01, (byte) 0x00, (byte) 0xff};
        public final static byte [] backward = new byte[]{(byte) 0xff, (byte) 0x00, (byte) 0x02, (byte) 0x00, (byte) 0xff};
        public final static byte [] right = new byte[]{(byte) 0xff, (byte) 0x00, (byte) 0x03, (byte) 0x00, (byte) 0xff};
        public final static byte [] left = new byte[]{(byte) 0xff, (byte) 0x00, (byte) 0x04, (byte) 0x00, (byte) 0xff};

        public static final byte [] l_f = new byte[] {(byte) 0xff, (byte) 0x00, (byte) 0x05, (byte) 0x00, (byte) 0xff};
        public static final byte [] l_b = new byte[] {(byte) 0xff, (byte) 0x00, (byte) 0x06, (byte) 0x00, (byte) 0xff};
        public static final byte [] r_f = new byte[] {(byte) 0xff, (byte) 0x00, (byte) 0x07, (byte) 0x00, (byte) 0xff};
        public static final byte [] r_b = new byte[] {(byte) 0xff, (byte) 0x00, (byte) 0x08, (byte) 0x00, (byte) 0xff};

        public final static byte [] stop = new byte[]{(byte) 0xff, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0xff};
    }

    public final static class host {
        public static final String server = "123.207.82.132";
    }

    public final static class port {
        public static final int song        = 8083;
        public static final int gesture     = 8080;
        public static final int instruct    = 8081;
    }

    public static Map<Integer,byte[]> instructMap = new HashMap<Integer, byte[]>();
    public static Map<Integer,String> songMap = new HashMap<Integer, String>();
    static {
        instructMap.put(0,Constant.instruct.stop);
        instructMap.put(1,Constant.instruct.forward);
        instructMap.put(2,Constant.instruct.backward);
        instructMap.put(3,Constant.instruct.right);
        instructMap.put(4,Constant.instruct.left);

        songMap.put(0,"stop");
        songMap.put(1,"forward");
        songMap.put(2,"backward");
        songMap.put(3,"right");
        songMap.put(4,"left");
    }
}
