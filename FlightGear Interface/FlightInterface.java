/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * FlightInterface.java
 *
 * Created on Jan 31, 2011, 5:03:46 PM
 */
package capstone;

import java.awt.Color;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.io.IOException;
import java.net.InetAddress;

/**
 *
 * @author root
 */
public class FlightInterface extends javax.swing.JFrame {

    public static DatagramSocket sendSock;
    public static DatagramSocket receiveSock;
    public static final int sendPort = 12345;
    public static final int receivePort = 12347;
    public static DatagramPacket receivePacket;
    public static DatagramPacket packet;
    public static InetAddress addr;
    private static final double KNOTS_TO_FPM_CONV = 101.268591;
    private boolean fineControl = true;
    public static double fake = 0;
    public static double destHeading = 0;
    public static double apHeading = 0;
    public Thread calcHeadingThread;
    public Runnable runnableRight;
    public Runnable runnableLeft;
    public Runnable r;
    public int K = 1; //Gain
    private static int n = 1;   //number of times a button is pressed, reset after each maneuver is completed.
    private static int right_n = 0;   //number of times a button is pressed, reset after each maneuver is completed.
    private static int left_n = 0;
    private static boolean right = false;
    private static boolean left = false;

    /** Creates new form FlightInterface */
    public FlightInterface() {
        initComponents();
        runnableRight = new Runnable() {
			@Override
			public void run() {
				boolean shouldWait = false;
				while (true) {
					double curHead = Double.parseDouble(getData("get double /orientation/heading-magnetic-deg\n"));
					fake = (K * Math.abs(curHead - destHeading));// % 360;

					if ((destHeading + fake) > (curHead + 178)){
						apHeading = (curHead + 178) % 360;
						sendData("set double /autopilot/settings/heading-bug-deg " + apHeading + "\n");
						System.out.println("apHeading_" + Thread.currentThread().getName() + " = " + apHeading);
						shouldWait = true;
						break;
					} else {
						apHeading = (destHeading + fake) % 360;
					}

					System.out.println("curHead = " + curHead);
					System.out.println("fake = " + fake);
					System.out.println("apHeading_" + Thread.currentThread().getName() + " = " + apHeading);
					System.out.println();

					sendData("set double /autopilot/settings/heading-bug-deg " + apHeading + "\n");
					try {
						Thread.sleep(1000);
					} catch (InterruptedException e) {
						e.printStackTrace();
						break;
					}
					if (Math.abs(Double.parseDouble(getData("get double /orientation/heading-magnetic-deg\n")) - apHeading) < .75) {
						//finished maneuver
						System.out.println("maneuver finished");
						left_n = 0;
						right_n = 0;
						left = false;
						right = false;
						break;
					}
				}

				if (shouldWait){
					while(true) {
						try {
							Thread.sleep(1000);
						} catch (InterruptedException e) {
							e.printStackTrace();
							break;
						}
						if (Math.abs(Double.parseDouble(getData("get double /orientation/heading-magnetic-deg\n")) - destHeading) < .75) {
							//finished maneuver
							System.out.println("maneuver finished");
							left_n = 0;
							right_n = 0;
							left = false;
							right = false;
							break;
						}
					}
				}

				System.out.println("finished loop");
			}
		};

		runnableLeft = new Runnable() {
			@Override
			public void run() {
				boolean shouldWait = false;
				while (true) {
					double curHead = Double.parseDouble(getData("get double /orientation/heading-magnetic-deg\n"));
					fake = (K * Math.abs(curHead - destHeading));// % 360;

					if ((destHeading - fake) < (curHead - 178)){
						apHeading = (curHead - 178) % 360;
						sendData("set double /autopilot/settings/heading-bug-deg " + apHeading + "\n");
						System.out.println("apHeading_" + Thread.currentThread().getName() + " = " + apHeading);
						shouldWait = true;
						break;
					} else {
						apHeading = (destHeading - fake) % 360;
					}

					System.out.println("curHead = " + curHead);
					System.out.println("fake = " + fake);
					System.out.println("apHeading_" + Thread.currentThread().getName() + " = " + apHeading);
					System.out.println();

					sendData("set double /autopilot/settings/heading-bug-deg " + apHeading + "\n");
					try {
						Thread.sleep(1000);
					} catch (InterruptedException e) {
						e.printStackTrace();
						break;
					}
					if (Math.abs(Double.parseDouble(getData("get double /orientation/heading-magnetic-deg\n")) - destHeading) < .75) {
						//finished maneuver
						System.out.println("maneuver finished");
						left_n = 0;
						right_n = 0;
						left = false;
						right = false;
						break;
					}
				}

				if (shouldWait){
					while(true) {
						try {
							Thread.sleep(1000);
						} catch (InterruptedException e) {
							e.printStackTrace();
							break;
						}
						if (Math.abs(Double.parseDouble(getData("get double /orientation/heading-magnetic-deg\n")) - destHeading) < .75) {
							//finished maneuver
							System.out.println("maneuver finished");
							left_n = 0;
							right_n = 0;
							left = false;
							right = false;
							break;
						}
					}
				}

				System.out.println("finished loop");
			}
		};
    }

    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jButton1 = new javax.swing.JButton();
        jButton2 = new javax.swing.JButton();
        jButton3 = new javax.swing.JButton();
        jButton4 = new javax.swing.JButton();
        jButton5 = new javax.swing.JButton();
        jButton6 = new javax.swing.JButton();
        jButton7 = new javax.swing.JButton();
        jButton8 = new javax.swing.JButton();
        jButton9 = new javax.swing.JButton();
        jButton10 = new javax.swing.JButton();
        jButton11 = new javax.swing.JButton();
        jButton12 = new javax.swing.JButton();
        jButton13 = new javax.swing.JButton();
        jButton14 = new javax.swing.JButton();
        jButton15 = new javax.swing.JButton();
        jButton16 = new javax.swing.JButton();
        jButton17 = new javax.swing.JButton();
        jButton18 = new javax.swing.JButton();
        jButton19 = new javax.swing.JButton();
        jButton20 = new javax.swing.JButton();
        jButton21 = new javax.swing.JButton();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setIconImages(null);
        setMinimumSize(new java.awt.Dimension(1024, 768));
        getContentPane().setLayout(null);

        jButton1.setBackground(new java.awt.Color(204, 0, 0));
        jButton1.setPreferredSize(new java.awt.Dimension(25, 25));
        jButton1.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButton1MouseClicked(evt);
            }
        });
        getContentPane().add(jButton1);
        jButton1.setBounds(490, 250, 25, 25);

        jButton2.setBackground(new java.awt.Color(204, 0, 0));
        jButton2.setPreferredSize(new java.awt.Dimension(25, 25));
        jButton2.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButton2MouseClicked(evt);
            }
        });
        getContentPane().add(jButton2);
        jButton2.setBounds(490, 370, 25, 25);

        jButton3.setBackground(new java.awt.Color(204, 0, 0));
        jButton3.setPreferredSize(new java.awt.Dimension(25, 25));
        jButton3.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButton3MouseClicked(evt);
            }
        });
        getContentPane().add(jButton3);
        jButton3.setBounds(550, 310, 25, 25);

        jButton4.setBackground(new java.awt.Color(204, 0, 0));
        jButton4.setPreferredSize(new java.awt.Dimension(25, 25));
        jButton4.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButton4MouseClicked(evt);
            }
        });
        getContentPane().add(jButton4);
        jButton4.setBounds(420, 310, 25, 25);

        jButton5.setBackground(new java.awt.Color(102, 0, 0));
        jButton5.setPreferredSize(new java.awt.Dimension(25, 25));
        jButton5.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButton5MouseClicked(evt);
            }
            public void mouseEntered(java.awt.event.MouseEvent evt) {
                jButton5MouseEntered(evt);
            }
        });
        getContentPane().add(jButton5);
        jButton5.setBounds(650, 310, 25, 25);

        jButton6.setBackground(new java.awt.Color(102, 0, 0));
        jButton6.setPreferredSize(new java.awt.Dimension(25, 25));
        jButton6.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButton6MouseClicked(evt);
            }
        });
        getContentPane().add(jButton6);
        jButton6.setBounds(320, 310, 25, 25);

        jButton7.setBackground(new java.awt.Color(102, 0, 0));
        jButton7.setPreferredSize(new java.awt.Dimension(25, 25));
        jButton7.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButton7MouseClicked(evt);
            }
        });
        getContentPane().add(jButton7);
        jButton7.setBounds(490, 150, 25, 25);

        jButton8.setBackground(new java.awt.Color(102, 0, 0));
        jButton8.setPreferredSize(new java.awt.Dimension(25, 25));
        jButton8.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButton8MouseClicked(evt);
            }
        });
        getContentPane().add(jButton8);
        jButton8.setBounds(490, 470, 25, 25);

        jButton9.setBackground(new java.awt.Color(102, 0, 0));
        jButton9.setPreferredSize(new java.awt.Dimension(25, 25));
        jButton9.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButton9MouseClicked(evt);
            }
        });
        getContentPane().add(jButton9);
        jButton9.setBounds(590, 410, 25, 25);

        jButton10.setBackground(new java.awt.Color(102, 0, 0));
        jButton10.setPreferredSize(new java.awt.Dimension(25, 25));
        jButton10.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButton10MouseClicked(evt);
            }
        });
        getContentPane().add(jButton10);
        jButton10.setBounds(590, 210, 25, 25);

        jButton11.setBackground(new java.awt.Color(102, 0, 0));
        jButton11.setPreferredSize(new java.awt.Dimension(25, 25));
        jButton11.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButton11MouseClicked(evt);
            }
        });
        getContentPane().add(jButton11);
        jButton11.setBounds(380, 210, 25, 25);

        jButton12.setBackground(new java.awt.Color(102, 0, 0));
        jButton12.setPreferredSize(new java.awt.Dimension(25, 25));
        jButton12.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButton12MouseClicked(evt);
            }
        });
        getContentPane().add(jButton12);
        jButton12.setBounds(380, 410, 25, 25);

        jButton13.setBackground(new java.awt.Color(204, 0, 0));
        jButton13.setPreferredSize(new java.awt.Dimension(25, 25));
        jButton13.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButton13MouseClicked(evt);
            }
        });
        jButton13.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton13ActionPerformed(evt);
            }
        });
        getContentPane().add(jButton13);
        jButton13.setBounds(440, 690, 25, 25);

        jButton14.setBackground(new java.awt.Color(204, 0, 0));
        jButton14.setPreferredSize(new java.awt.Dimension(25, 25));
        jButton14.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButton14MouseClicked(evt);
            }
        });
        getContentPane().add(jButton14);
        jButton14.setBounds(490, 610, 25, 25);

        jButton15.setBackground(new java.awt.Color(204, 0, 0));
        jButton15.setPreferredSize(new java.awt.Dimension(25, 25));
        jButton15.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButton15MouseClicked(evt);
            }
        });
        getContentPane().add(jButton15);
        jButton15.setBounds(280, 610, 25, 25);

        jButton16.setBackground(new java.awt.Color(204, 0, 0));
        jButton16.setPreferredSize(new java.awt.Dimension(25, 25));
        jButton16.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButton16MouseClicked(evt);
            }
        });
        getContentPane().add(jButton16);
        jButton16.setBounds(280, 570, 25, 25);

        jButton17.setBackground(new java.awt.Color(204, 0, 0));
        jButton17.setPreferredSize(new java.awt.Dimension(25, 25));
        jButton17.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButton17MouseClicked(evt);
            }
        });
        getContentPane().add(jButton17);
        jButton17.setBounds(680, 570, 25, 25);

        jButton18.setBackground(new java.awt.Color(204, 0, 0));
        jButton18.setPreferredSize(new java.awt.Dimension(25, 25));
        jButton18.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButton18MouseClicked(evt);
            }
        });
        getContentPane().add(jButton18);
        jButton18.setBounds(470, 650, 25, 25);

        jButton19.setBackground(new java.awt.Color(204, 0, 0));
        jButton19.setPreferredSize(new java.awt.Dimension(25, 25));
        jButton19.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButton19MouseClicked(evt);
            }
        });
        getContentPane().add(jButton19);
        jButton19.setBounds(510, 650, 25, 25);

        jButton20.setBackground(java.awt.Color.green);
        jButton20.setPreferredSize(new java.awt.Dimension(25, 25));
        jButton20.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButton20MouseClicked(evt);
            }
        });
        getContentPane().add(jButton20);
        jButton20.setBounds(540, 690, 25, 25);

        jButton21.setBackground(new java.awt.Color(204, 0, 0));
        jButton21.setPreferredSize(new java.awt.Dimension(25, 25));
        jButton21.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButton21MouseClicked(evt);
            }
        });
        getContentPane().add(jButton21);
        jButton21.setBounds(680, 610, 25, 25);

        pack();
    }// </editor-fold>//GEN-END:initComponents

    //down0
    private void jButton2MouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton2MouseClicked
        if (fineControl) {
            String elevator = getData("get double /controls/flight/elevator\n");
            Double elevatorPosition = Double.parseDouble(elevator);
            if (elevatorPosition <= .95) {
                elevatorPosition += .05;
                elevator = elevatorPosition.toString();
                sendData("set double /controls/flight/elevator " + elevator + "\n");
            }
            String sendStuff = "set double /autopilot/settings/vertical-speed-fpm 0\n";
            sendData(sendStuff);
            sendData("set string /autopilot/locks/altitude off\n");

        } else {
            String vertSpeed = getData("get double /autopilot/settings/vertical-speed-fpm\n");
            String airSpeedKnots = getData("get double /instrumentation/airspeed-indicator/true-speed-kt\n");

            double airSpeedFpm = Double.parseDouble(airSpeedKnots) * KNOTS_TO_FPM_CONV;
            double newVertSpeed = Double.parseDouble(vertSpeed) - .052 * airSpeedFpm;
            System.out.println("new vertspeed = " + newVertSpeed);

            String sendStuff = "set double /autopilot/settings/vertical-speed-fpm " + newVertSpeed + "\n";
            sendData(sendStuff);
            
            sendData("set string /autopilot/locks/altitude vertical-speed-hold\n");
        }
    }//GEN-LAST:event_jButton2MouseClicked

    //right0
    private void jButton3MouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton3MouseClicked

    	if (calcHeadingThread != null && calcHeadingThread.isAlive()) {
            calcHeadingThread.interrupt();
        }
    	
        double curHead = Double.parseDouble(getData("get double /orientation/heading-magnetic-deg\n"));
        if(!right && !left)
        {
            sendData("set double /autopilot/settings/heading-bug-deg " + curHead + "\n");
        }

        // String altitude = getData("get double /position/altitude-ft\n");
        if (fineControl) {
            String aileron = getData("get double /controls/flight/aileron\n");
            Double aileronPosition = Double.parseDouble(aileron);
            if (aileronPosition <= .95) {
                aileronPosition += .05;
                aileron = aileronPosition.toString();
                sendData("set double /controls/flight/aileron " + aileron + "\n");
            }

        } else {
            sendData("set string /autopilot/locks/heading dg-heading-hold\n");
            
            String apHead_str = getData("get double /autopilot/settings/heading-bug-deg\n");

            double error = Math.abs(Double.parseDouble(apHead_str) - curHead);
            if (error < 2) {
                error = 0;
            }

            right = true;
            right_n++;

            if (left) {
                //destHeading = (curHead - error + 3*right_n);// % 360;
                destHeading = destHeading - error + 3 * right_n;
                if (right_n >= left_n) {
                    left = false;
                    left_n = 0;
                }

            } else {
                destHeading = (curHead + error + 3 * right_n);// % 360;
            }

            System.out.println();
            System.out.println("start heading = " + curHead);
            System.out.println("error = " + error);
            System.out.println("destheading = " + destHeading);

            (calcHeadingThread = new Thread(runnableRight, "right_" + Integer.toString(right_n))).start();
        }

    }//GEN-LAST:event_jButton3MouseClicked

    //right1
    private void jButton5MouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton5MouseClicked
    	if (calcHeadingThread != null && calcHeadingThread.isAlive()) {
            calcHeadingThread.interrupt();
        }
    	
        double curHead = Double.parseDouble(getData("get double /orientation/heading-magnetic-deg\n"));
        if(!left && !right)
        {
            sendData("set double /autopilot/settings/heading-bug-deg " + curHead + "\n");
        }
        //String altitude = getData("get double /position/altitude-ft\n");
        if (fineControl) {
            String aileron = getData("get double /controls/flight/aileron\n");
            Double aileronPosition = Double.parseDouble(aileron);
            if (aileronPosition <= .9) {
                aileronPosition += .1;
                aileron = aileronPosition.toString();
                sendData("set double /controls/flight/aileron " + aileron + "\n");
            } else {
                sendData("set string /autopilot/locks/heading dg-heading-hold\n");
                //double curHead = Double.parseDouble(getData("get double /orientation/heading-magnetic-deg\n"));
                String apHead_str = getData("get double /autopilot/settings/heading-bug-deg\n");

                double error = Math.abs(Double.parseDouble(apHead_str) - curHead);
                if (error < 2) {
                    error = 0;
                }

                right = true;
                right_n++;

                if (left) {
                    destHeading = destHeading - error + 6 * right_n;
                    if (right_n >= left_n) {
                        left = false;
                        left_n = 0;
                    }
                } else {
                    destHeading = (curHead + error + 6 * right_n);// % 360;
                }

                System.out.println();
                System.out.println("start heading = " + curHead);
                System.out.println("error = " + error);
                System.out.println("destheading = " + destHeading);

                (calcHeadingThread = new Thread(runnableRight, "right_" + Integer.toString(right_n))).start();
            }
    }//GEN-LAST:event_jButton5MouseClicked
    }

    //left1
    private void jButton6MouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton6MouseClicked
        // TODO add your handling code here:
    	if (calcHeadingThread != null && calcHeadingThread.isAlive()) {
            calcHeadingThread.interrupt();
        }
    	
        double curHead = Double.parseDouble(getData("get double /orientation/heading-magnetic-deg\n"));
        if(!left && !right)
        {
            sendData("set double /autopilot/settings/heading-bug-deg " + curHead + "\n");
        }
        if (fineControl) {
            String aileron = getData("get double /controls/flight/aileron\n");
            Double aileronPosition = Double.parseDouble(aileron);
            if (aileronPosition >= -.9) {
                aileronPosition -= .1;
                aileron = aileronPosition.toString();
                sendData("set double /controls/flight/aileron " + aileron + "\n");
            }
        } else {
            sendData("set string /autopilot/locks/heading dg-heading-hold\n");
            //double curHead = Double.parseDouble(getData("get double /orientation/heading-magnetic-deg\n"));
            String apHead_str = getData("get double /autopilot/settings/heading-bug-deg\n");

            double error = Math.abs(Double.parseDouble(apHead_str) - curHead);
            if (error < 2) {
                error = 0;
            }

            left = true;
            left_n++;

            if (right) {
                destHeading = destHeading + error - 6 * left_n;
                if (left_n >= right_n) {
                    right = false;
                    right_n = 0;
                }
            } else {
                destHeading = (curHead - error - 6 * left_n);
            }

            System.out.println();
            System.out.println("start heading = " + curHead);
            System.out.println("error = " + error);
            System.out.println("destheading = " + destHeading);

            (calcHeadingThread = new Thread(runnableLeft, "left_" + Integer.toString(left_n))).start();
        }

    }//GEN-LAST:event_jButton6MouseClicked

    //left0
    private void jButton4MouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton4MouseClicked
        // TODO add your handling code here:
        //String resultData = "";
        //String altitude = getData("get double /position/altitude-ft\n");
    	if (calcHeadingThread != null && calcHeadingThread.isAlive()) {
            calcHeadingThread.interrupt();
        }
    	
        double curHead = Double.parseDouble(getData("get double /orientation/heading-magnetic-deg\n"));
        if(!left && !right)
        {
            sendData("set double /autopilot/settings/heading-bug-deg " + curHead + "\n");
        }
        if (fineControl) {

            String aileron = getData("get double /controls/flight/aileron\n");
            Double aileronPosition = Double.parseDouble(aileron);
            if (aileronPosition >= -.95) {
                aileronPosition -= .05;
                aileron = aileronPosition.toString();
                sendData("set double /controls/flight/aileron " + aileron + "\n");
            }
            //String sendStuff = "set double /autopilot/settings/vertical-speed-fpm 0\n";
            //sendData(sendStuff);
            //sendData("set string /autopilot/locks/heading off\n");
        } else {
            sendData("set string /autopilot/locks/heading dg-heading-hold\n");
            //double curHead = Double.parseDouble(getData("get double /orientation/heading-magnetic-deg\n"));
            String apHead_str = getData("get double /autopilot/settings/heading-bug-deg\n");

            double error = Math.abs(Double.parseDouble(apHead_str) - curHead);
            if (error < 2) {
                error = 0;
            }

            left = true;
            left_n++;

            if (right) {
                destHeading = destHeading + error - 3 * left_n;
                if (left_n >= right_n) {
                    right = false;
                    right_n = 0;
                }
            } else {
                destHeading = (curHead - error - 3 * left_n);
            }

            System.out.println();
            System.out.println("start heading = " + curHead);
            System.out.println("error = " + error);
            System.out.println("destheading = " + destHeading);

            (calcHeadingThread = new Thread(runnableLeft, "left_" + Integer.toString(left_n))).start();
        }
    }//GEN-LAST:event_jButton4MouseClicked

    //gear
    private void jButton13MouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton13MouseClicked
        // TODO add your handling code here:
        String resultData = "";
        try {

            Double gear = new Double(0);
            resultData = getData("get bool /controls/gear/gear-down\n");
            gear = Double.parseDouble(resultData);
            if (gear == 1) {
                sendData("set bool /controls/gear/gear-down false\n");
            } else if (gear == 0) {
                sendData("set bool /controls/gear/gear-down true\n");
            } else {
                System.out.println("Problem with landing gear...");
            }
        } catch (Exception e) {
            System.out.println("Problem sending the packet.");
            e.printStackTrace();
        }
    }//GEN-LAST:event_jButton13MouseClicked

    //up0
    private void jButton1MouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton1MouseClicked

        String altitude = getData("get double /position/altitude-ft\n");
        System.out.println(altitude);
        if (fineControl) {
            String elevator = getData("get double /controls/flight/elevator\n");
            Double elevatorPosition = Double.parseDouble(elevator);
            if (elevatorPosition >= -.95) {
                elevatorPosition -= .05;
                elevator = elevatorPosition.toString();
                sendData("set double /controls/flight/elevator " + elevator + "\n");
            }
            String sendStuff = "set double /autopilot/settings/vertical-speed-fpm 0\n";
            sendData(sendStuff);
            sendData("set string /autopilot/locks/altitude off\n");
        } else {
            sendData("set string /autopilot/locks/altitude vertical-speed-hold\n");

            String vertSpeed = getData("get double /autopilot/settings/vertical-speed-fpm\n");
            String airSpeedKnots = getData("get double /instrumentation/airspeed-indicator/true-speed-kt\n");

            double airSpeedFpm = Double.parseDouble(airSpeedKnots) * KNOTS_TO_FPM_CONV;
            double newVertSpeed = Double.parseDouble(vertSpeed) + .052 * airSpeedFpm;
            System.out.println("new vertspeed = " + newVertSpeed);

            String sendStuff = "set double /autopilot/settings/vertical-speed-fpm " + newVertSpeed + "\n";
            sendData(sendStuff);
        }
    }//GEN-LAST:event_jButton1MouseClicked

    //wing leveler
    private void jButton14MouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton14MouseClicked

        sendData("set double /controls/flight/elevator 0\n");
        sendData("set double /controls/flight/aileron 0\n");
        sendData("set double /controls/flight/rudder 0\n");



        if (!fineControl) {
            if (calcHeadingThread != null && calcHeadingThread.isAlive()) {
                calcHeadingThread.interrupt();
            }

            String currentHeading = getData("get double /orientation/heading-magnetic-deg\n");
            sendData("set double /autopilot/settings/heading-bug-deg " + currentHeading);  //new line already included from getData in previous line
            sendData("set string /autopilot/locks/heading dg-heading-hold\n");

            sendData("set double /autopilot/settings/vertical-speed-fpm 0\n");
            sendData("set string /autopilot/locks/altitude vertical-speed-hold\n");
        }
        right_n = 0;
        left_n = 0;
       

    }//GEN-LAST:event_jButton14MouseClicked

    //throttle down
    private void jButton15MouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton15MouseClicked
        // TODO add your handling code here:
        String resultData = "";
        try {
            Double throttle = new Double(0);
            resultData = getData("get double /controls/engines/engine/throttle\n");
            throttle = Double.parseDouble(resultData);
            if (throttle >= .1) {
                throttle = throttle - .1;
            }
            String sendThrottle = "set double /controls/engines/engine/throttle " + throttle.toString() + "\n";
            String sendThrottle2 = "set double /controls/engines/engine[1]/throttle " + throttle.toString() + "\n";
            String sendThrottle3 = "set double /controls/engines/engine[2]/throttle " + throttle.toString() + "\n";

            sendData(sendThrottle);
            sendData(sendThrottle2);
            sendData(sendThrottle3);

        } catch (Exception e) {
            System.out.println("Problem sending the packet.");
            e.printStackTrace();
        }

    }//GEN-LAST:event_jButton15MouseClicked

    //throttle up
    private void jButton16MouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton16MouseClicked
        // TODO add your handling code here:
        // TODO add your handling code here:
        String resultData = "";
        try {
            Double throttle = new Double(0);
            resultData = getData("get double /controls/engines/engine/throttle\n");
            throttle = Double.parseDouble(resultData);
            if (throttle <= .9) {
                throttle = throttle + .1;
            }
            String sendThrottle = "set double /controls/engines/engine/throttle " + throttle.toString() + "\n";
            String sendThrottle2 = "set double /controls/engines/engine[1]/throttle " + throttle.toString() + "\n";
            String sendThrottle3 = "set double /controls/engines/engine[2]/throttle " + throttle.toString() + "\n";

            sendData(sendThrottle);
            sendData(sendThrottle2);
            sendData(sendThrottle3);

        } catch (Exception e) {
            System.out.println("Problem sending the packet.");
            e.printStackTrace();
        }
    }//GEN-LAST:event_jButton16MouseClicked

    //brake
    private void jButton17MouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton17MouseClicked
        // TODO add your handling code here:

        String resultData = "";
        Double trueSpeed = new Double(0);
        resultData = getData("get double /velocities/groundspeed-kt\n");
        trueSpeed = Double.parseDouble(resultData);

        if (trueSpeed < 5) {
            Double gear = new Double(0);
            resultData = "";
            resultData = getData("get bool /controls/gear/brake-parking\n");
            gear = Double.parseDouble(resultData);
            if (gear == 1) {
                sendData("set bool /controls/gear/brake-parking false\n");
            } else if (gear == 0) {
                sendData("set bool /controls/gear/brake-parking true\n");
            } else {
                System.out.println("Problem with parking break...");
            }
        } else {
            Double speedBrake = new Double(0);
            resultData = "";
            resultData = getData("get double /controls/flight/speedbrake\n");
            speedBrake = Double.parseDouble(resultData);
            if (speedBrake == 0) {
                sendData("set double /controls/flight/speedbrake 1\n");
                sendData("set double /controls/flight/spoilers 1\n");
                sendData("set double /controls/gear/brake-left 1\n");
                sendData("set double /controls/gear/brake-right 1\n");
            } else {
                sendData("set double /controls/flight/speedbrake 0\n");
                sendData("set double /controls/flight/spoilers 0\n");
                sendData("set double /controls/gear/brake-left 0\n");
                sendData("set double /controls/gear/brake-right 0\n");
            }
        }
    }//GEN-LAST:event_jButton17MouseClicked

    //up1
    private void jButton7MouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton7MouseClicked
        String altitude = getData("get double /position/altitude-ft\n");
        if (fineControl) {
            String elevator = getData("get double /controls/flight/elevator\n");
            Double elevatorPosition = Double.parseDouble(elevator);
            if (elevatorPosition >= -.9) {
                elevatorPosition -= .1;
                elevator = elevatorPosition.toString();
                sendData("set double /controls/flight/elevator " + elevator + "\n");
            }
            String sendStuff = "set double /autopilot/settings/vertical-speed-fpm 0\n";
            sendData(sendStuff);
            sendData("set string /autopilot/locks/altitude off\n");
        } else {
            //sendData("set bool /controls/autoflight/autopilot[1]/engage true\n");
            sendData("set string /autopilot/locks/altitude vertical-speed-hold\n");

            String vertSpeed = getData("get double /autopilot/settings/vertical-speed-fpm\n");
            String airSpeedKnots = getData("get double /instrumentation/airspeed-indicator/true-speed-kt\n");

            double airSpeedFpm = Double.parseDouble(airSpeedKnots) * KNOTS_TO_FPM_CONV;
            double newVertSpeed = Double.parseDouble(vertSpeed) + 0.176 * airSpeedFpm;
            System.out.println("new vertspeed = " + newVertSpeed);

            String sendStuff = "set double /autopilot/settings/vertical-speed-fpm " + newVertSpeed + "\n";
            sendData(sendStuff);
        }
    }//GEN-LAST:event_jButton7MouseClicked

    //down1
    private void jButton8MouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton8MouseClicked

        if (fineControl) {
            String elevator = getData("get double /controls/flight/elevator\n");
            Double elevatorPosition = Double.parseDouble(elevator);
            if (elevatorPosition <= .9) {
                elevatorPosition += .1;
                elevator = elevatorPosition.toString();
                sendData("set double /controls/flight/elevator " + elevator + "\n");
            }
            String sendStuff = "set double /autopilot/settings/vertical-speed-fpm 0\n";
            sendData(sendStuff);
            sendData("set string /autopilot/locks/altitude off\n");

        } else {
            //sendData("set bool /controls/autoflight/autopilot[1]/engage true\n");
            sendData("set string /autopilot/locks/altitude vertical-speed-hold\n");

            String vertSpeed = getData("get double /autopilot/settings/vertical-speed-fpm\n");
            String airSpeedKnots = getData("get double /instrumentation/airspeed-indicator/true-speed-kt\n");

            double airSpeedFpm = Double.parseDouble(airSpeedKnots) * KNOTS_TO_FPM_CONV;
            double newVertSpeed = Double.parseDouble(vertSpeed) - 0.176 * airSpeedFpm;
            System.out.println("new vertspeed = " + newVertSpeed);

            String sendStuff = "set double /autopilot/settings/vertical-speed-fpm " + newVertSpeed + "\n";
            sendData(sendStuff);
        }

    }//GEN-LAST:event_jButton8MouseClicked

    //down_right
    private void jButton9MouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton9MouseClicked
        System.out.println("button 9");

        if (fineControl) {
            return;
        }

        if (calcHeadingThread != null && calcHeadingThread.isAlive()) {
            calcHeadingThread.interrupt();
        }

        sendData("set string /autopilot/locks/altitude vertical-speed-hold\n");

        String vertSpeed = getData("get double /autopilot/settings/vertical-speed-fpm\n");
        String airSpeedKnots = getData("get double /instrumentation/airspeed-indicator/true-speed-kt\n");

        double airSpeedFpm = Double.parseDouble(airSpeedKnots) * KNOTS_TO_FPM_CONV;
        double newVertSpeed = Double.parseDouble(vertSpeed) - .052 * airSpeedFpm;
        System.out.println("new vertspeed = " + newVertSpeed);

        String sendStuff = "set double /autopilot/settings/vertical-speed-fpm " + newVertSpeed + "\n";
        sendData(sendStuff);

        sendData("set string /autopilot/locks/heading dg-heading-hold\n");
        double curHead = Double.parseDouble(getData("get double /orientation/heading-magnetic-deg\n"));
        String apHead_str = getData("get double /autopilot/settings/heading-bug-deg\n");

        double error = Math.abs(Double.parseDouble(apHead_str) - curHead);
        if (error < 2) {
            error = 0;
        }

        right = true;
        right_n++;

        if (left) {
            destHeading = destHeading - error + 3 * right_n;
            if (right_n >= left_n) {
                left = false;
                left_n = 0;
            }
        } else {
            destHeading = (curHead + error + 3 * right_n);// % 360;
        }

        System.out.println();
        System.out.println("start heading = " + curHead);
        System.out.println("error = " + error);
        System.out.println("destheading = " + destHeading);

        (calcHeadingThread = new Thread(runnableRight, "right_" + Integer.toString(right_n))).start();
    }//GEN-LAST:event_jButton9MouseClicked

    
    private void jButton10MouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton10MouseClicked
        System.out.println("button 10");

        if (fineControl) {
            return;
        }

        if (calcHeadingThread != null && calcHeadingThread.isAlive()) {
            calcHeadingThread.interrupt();
        }

        sendData("set string /autopilot/locks/altitude vertical-speed-hold\n");

        String vertSpeed = getData("get double /autopilot/settings/vertical-speed-fpm\n");
        String airSpeedKnots = getData("get double /instrumentation/airspeed-indicator/true-speed-kt\n");

        double airSpeedFpm = Double.parseDouble(airSpeedKnots) * KNOTS_TO_FPM_CONV;
        double newVertSpeed = Double.parseDouble(vertSpeed) + .052 * airSpeedFpm;
        System.out.println("new vertspeed = " + newVertSpeed);

        String sendStuff = "set double /autopilot/settings/vertical-speed-fpm " + newVertSpeed + "\n";
        sendData(sendStuff);

        //start right control
        sendData("set string /autopilot/locks/heading dg-heading-hold\n");
        double curHead = Double.parseDouble(getData("get double /orientation/heading-magnetic-deg\n"));
        String apHead_str = getData("get double /autopilot/settings/heading-bug-deg\n");

        double error = Math.abs(Double.parseDouble(apHead_str) - curHead);
        if (error < 2) {
            error = 0;
        }

        right = true;
        right_n++;

        if (left) {
            destHeading = destHeading - error + 3 * right_n;
            if (right_n >= left_n) {
                left = false;
                left_n = 0;
            }
        } else {
            destHeading = (curHead + error + 3 * right_n);// % 360;
        }

        System.out.println();
        System.out.println("start heading = " + curHead);
        System.out.println("error = " + error);
        System.out.println("destheading = " + destHeading);

        (calcHeadingThread = new Thread(runnableRight, "right_" + Integer.toString(right_n))).start();
    }//GEN-LAST:event_jButton10MouseClicked

    private void jButton11MouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton11MouseClicked
        System.out.println("button 11");

        if (fineControl) {
            return;
        }

        if (calcHeadingThread != null && calcHeadingThread.isAlive()) {
            calcHeadingThread.interrupt();
        }

        sendData("set string /autopilot/locks/altitude vertical-speed-hold\n");

        String vertSpeed = getData("get double /autopilot/settings/vertical-speed-fpm\n");
        String airSpeedKnots = getData("get double /instrumentation/airspeed-indicator/true-speed-kt\n");

        double airSpeedFpm = Double.parseDouble(airSpeedKnots) * KNOTS_TO_FPM_CONV;
        double newVertSpeed = Double.parseDouble(vertSpeed) + .052 * airSpeedFpm;
        System.out.println("new vertspeed = " + newVertSpeed);

        String sendStuff = "set double /autopilot/settings/vertical-speed-fpm " + newVertSpeed + "\n";
        sendData(sendStuff);

        //start left control
        sendData("set string /autopilot/locks/heading dg-heading-hold\n");
        double curHead = Double.parseDouble(getData("get double /orientation/heading-magnetic-deg\n"));
        String apHead_str = getData("get double /autopilot/settings/heading-bug-deg\n");

        double error = Math.abs(Double.parseDouble(apHead_str) - curHead);
        if (error < 2) {
            error = 0;
        }

        left = true;
        left_n++;

        if (right) {
            destHeading = destHeading + error - 3 * left_n;
            if (left_n >= right_n) {
                right = false;
                right_n = 0;
            }
        } else {
            destHeading = (curHead - error - 3 * left_n);
        }

        System.out.println();
        System.out.println("start heading = " + curHead);
        System.out.println("error = " + error);
        System.out.println("destheading = " + destHeading);

        (calcHeadingThread = new Thread(runnableLeft, "left_" + Integer.toString(left_n))).start();
    }//GEN-LAST:event_jButton11MouseClicked

    private void jButton12MouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton12MouseClicked
        System.out.println("button 12");

        if (fineControl) {
            return;
        }

        if (calcHeadingThread != null && calcHeadingThread.isAlive()) {
            calcHeadingThread.interrupt();
        }

        sendData("set string /autopilot/locks/altitude vertical-speed-hold\n");

        String vertSpeed = getData("get double /autopilot/settings/vertical-speed-fpm\n");
        String airSpeedKnots = getData("get double /instrumentation/airspeed-indicator/true-speed-kt\n");

        double airSpeedFpm = Double.parseDouble(airSpeedKnots) * KNOTS_TO_FPM_CONV;
        double newVertSpeed = Double.parseDouble(vertSpeed) - .052 * airSpeedFpm;
        System.out.println("new vertspeed = " + newVertSpeed);

        String sendStuff = "set double /autopilot/settings/vertical-speed-fpm " + newVertSpeed + "\n";
        sendData(sendStuff);

        //start left control
        sendData("set string /autopilot/locks/heading dg-heading-hold\n");
        double curHead = Double.parseDouble(getData("get double /orientation/heading-magnetic-deg\n"));
        String apHead_str = getData("get double /autopilot/settings/heading-bug-deg\n");

        double error = Math.abs(Double.parseDouble(apHead_str) - curHead);
        if (error < 2) {
            error = 0;
        }

        left = true;
        left_n++;

        if (right) {
            destHeading = destHeading + error - 3 * left_n;
            if (left_n >= right_n) {
                right = false;
                right_n = 0;
            }
        } else {
            destHeading = (curHead - error - 3 * left_n);
        }

        System.out.println();
        System.out.println("start heading = " + curHead);
        System.out.println("error = " + error);
        System.out.println("destheading = " + destHeading);

        (calcHeadingThread = new Thread(runnableLeft, "left_" + Integer.toString(left_n))).start();
    }//GEN-LAST:event_jButton12MouseClicked

    //rudder minus
    private void jButton18MouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton18MouseClicked
        if (fineControl) {
            //rudder stuff
            System.out.println("Entered Rudder Loop");
            String rudder = getData("get double /controls/flight/rudder\n");
            System.out.println("Rudder as a string: " + rudder);

            Double rudderPosition = Double.parseDouble(rudder);
            System.out.println("Rudder as a double%d" + rudderPosition);
            if (rudderPosition >= -.95) {
                rudderPosition -= .05;
                rudder = rudderPosition.toString();
                System.out.println("sending rudder position: " + rudder);
                sendData("set double /controls/flight/rudder " + rudder + "\n");
            }
            //String sendStuff = "set double /autopilot/settings/vertical-speed-fpm 0\n";
            //sendData(sendStuff);
            sendData("set string /autopilot/locks/heading off\n");

            //steering stuff
            String steering = getData("get double /controls/gear/steering\n");

            Double steeringPosition = Double.parseDouble(steering);
            if (steeringPosition >= -68) {
                steeringPosition -= 2;
                steering = steeringPosition.toString();
                sendData("set double /controls/gear/steering " + steering + "\n");
            }
        }
    }//GEN-LAST:event_jButton18MouseClicked

    //rudder
    private void jButton19MouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton19MouseClicked
        if (fineControl) {
            //aileron stuff
            String rudder = getData("get double /controls/flight/rudder\n");
            Double rudderPosition = Double.parseDouble(rudder);
            if (rudderPosition <= .95) {
                rudderPosition += .05;
                rudder = rudderPosition.toString();
                sendData("set double /controls/flight/rudder " + rudder + "\n");
            }
            //String sendStuff = "set double /autopilot/settings/heading-bug-deg 0\n";
            //sendData(sendStuff);
            sendData("set string /autopilot/locks/heading off\n");

            //steering stuff
            String steering = getData("get double /controls/gear/steering\n");

            Double steeringPosition = Double.parseDouble(steering);
            if (steeringPosition <= 68) {
                steeringPosition += 2;
                steering = steeringPosition.toString();
                sendData("set double /controls/gear/steering " + steering + "\n");
            }
        }
    }//GEN-LAST:event_jButton19MouseClicked

    //fine control
    private void jButton20MouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton20MouseClicked
        sendData("set double /controls/flight/elevator 0\n");
        sendData("set double /controls/flight/aileron 0\n");
        sendData("set double /controls/flight/rudder 0\n");
        if (fineControl) {
            sendData("set double /logging/log/entry[1]/enabled 1\n");
            fineControl = false;
            jButton20.setBackground(Color.red);
            String headingString = "";
            headingString = getData("get string /orientation/heading-magnetic-deg\n");
            System.out.println(headingString);
            sendData("set string /autopilot/settings/heading-bug-deg " + headingString + "\n");
        } else {
            sendData("set double /logging/log/entry[1]/enabled 0\n");
            if (calcHeadingThread != null && calcHeadingThread.isAlive()) {
                calcHeadingThread.interrupt();
            }
            fineControl = true;
            sendData("set string /autopilot/locks/heading off\n");
            sendData("set string /autopilot/locks/altitude off\n");
            jButton20.setBackground(Color.green);
            right = false;
            right_n = 0;
            left = false;
            left_n = 0;
        }
    }//GEN-LAST:event_jButton20MouseClicked

    private void jButton21MouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton21MouseClicked
        String flaps = getData("get double /controls/flight/flapscommand\n");
        Double flapsCommand = Double.parseDouble(flaps);
        if (flapsCommand == 1) {
            sendData("set double /controls/flight/flapscommand 0\n");
            sendData("set double /controls/flight/mainflaps 0\n");
            sendData("set double /controls/flight/slats 0\n");
            sendData("set double /controls/flight/auxflaps 0\n");
            sendData("set double /controls/flight/yasim-spoilers .14\n");
        } else {
            sendData("set double /controls/flight/flapscommand 1\n");
            sendData("set double /controls/flight/mainflaps 1\n");
            sendData("set double /controls/flight/slats 1\n");
            sendData("set double /controls/flight/auxflaps 1\n");
            sendData("set double /controls/flight/yasim-spoilers .067\n");
        }
    }//GEN-LAST:event_jButton21MouseClicked

    private void jButton13ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton13ActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_jButton13ActionPerformed

    private void jButton5MouseEntered(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton5MouseEntered
        // TODO add your handling code here:
    }//GEN-LAST:event_jButton5MouseEntered

    private void sendData(String setData) {
        try {
            String initialize = "0";
            byte[] sendBuffer = setData.getBytes();
            packet = new DatagramPacket(sendBuffer, sendBuffer.length);
            sendSock.send(packet);
            sendBuffer = initialize.getBytes();

        } catch (Exception e) {
            System.out.println("Problem sending the packet.");
            e.printStackTrace();
        }
    }

    private String getData(String getData) {
        String resultData = "";
        try {
            String initialize = "0";
            byte[] sendBuffer = getData.getBytes();
            packet = new DatagramPacket(sendBuffer, sendBuffer.length);
            byte[] receiveBuffer = new byte[2048];
            receivePacket = new DatagramPacket(receiveBuffer, receiveBuffer.length);
            sendBuffer = initialize.getBytes();
            sendSock.send(packet);
            receiveSock.receive(receivePacket);
            resultData = new String(receivePacket.getData());
            receiveBuffer = initialize.getBytes();
        } catch (Exception e) {
            System.out.println("Problem sending the packet.");
            e.printStackTrace();
        }
        System.out.println("In getData function, return result data" + resultData);
        return resultData;
    }

    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {

        try {
            java.awt.EventQueue.invokeLater(new Runnable() {

                public void run() {
                    new FlightInterface().setVisible(true);
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }

        try {

            addr = InetAddress.getByName("localhost");
            sendSock = new DatagramSocket();
            receiveSock = new DatagramSocket(receivePort);
            sendSock.connect(addr, sendPort);
        } catch (IOException e) {
            e.printStackTrace();
            System.out.println("Can't connect to localhost on port " + sendPort + ". Exiting...");
            System.exit(0);
        }

        System.out.println("Connected to localhost:" + sendPort);

    }
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton jButton1;
    private javax.swing.JButton jButton10;
    private javax.swing.JButton jButton11;
    private javax.swing.JButton jButton12;
    private javax.swing.JButton jButton13;
    private javax.swing.JButton jButton14;
    private javax.swing.JButton jButton15;
    private javax.swing.JButton jButton16;
    private javax.swing.JButton jButton17;
    private javax.swing.JButton jButton18;
    private javax.swing.JButton jButton19;
    private javax.swing.JButton jButton2;
    private javax.swing.JButton jButton20;
    private javax.swing.JButton jButton21;
    private javax.swing.JButton jButton3;
    private javax.swing.JButton jButton4;
    private javax.swing.JButton jButton5;
    private javax.swing.JButton jButton6;
    private javax.swing.JButton jButton7;
    private javax.swing.JButton jButton8;
    private javax.swing.JButton jButton9;
    // End of variables declaration//GEN-END:variables
}
