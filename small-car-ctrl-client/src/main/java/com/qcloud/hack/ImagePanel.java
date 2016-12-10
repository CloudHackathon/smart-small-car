package com.qcloud.hack;

import javax.swing.*;
import java.awt.*;


/**
 * @author lvyahui (lvyahui8@gmail.com,lvyahui8@126.com)
 * @since 2016/12/10 14:36
 */
public class ImagePanel extends JPanel {
    Image image;

    public void setImage(Image image) {
        this.image = image;
        repaint();
    }

    @Override
    protected void paintComponent(Graphics g) {
        if (image == null) {
            return;
        }
        super.paintComponent(g);
        g.drawImage(image, 0, 0, null);
    }
}
