package com.example.bedcontrol;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.widget.ImageView;
import android.widget.SeekBar;
import android.widget.Toast;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;

import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.ScheduledExecutorService;

import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;


import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;



public class MainActivity extends AppCompatActivity {

    /*******ID控件变量初始化***********/
    private ImageView ID_IMAGE_0;
    private SeekBar ID_SeekBar_0;
    int IMAGE0_flag = 0;

    /*******MQTT协议变量初始化********/
    private MqttClient client;
    private MqttConnectOptions options;
    private String host = "tcp://mqtt.dengsanpeng.com:1883";
    private String userName = "zhuchen";
    private String passWord = "test";

    private String mqtt_id = "387646981"; //定义成自己的QQ号  切记！不然会掉线！！！
    private String mqtt_sub_topic = "HealthyBed/Motor1"; //为了保证你不受到别人的消息  哈哈
    private String mqtt_pub_topic = "HealthyBed/Motor1"; //为了保证你不受到别人的消息  哈哈  自己QQ好后面加 _PC
    private ScheduledExecutorService scheduler;
    private Handler handler;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


        ui_init();//寻找控件ID




        /***********IMAGE0单击方法*********/

        ID_IMAGE_0.setOnClickListener(new View.OnClickListener() {

                                          @Override
                                          public void onClick(View v) {



                                              if (IMAGE0_flag == 0) {
                                                  ID_IMAGE_0.setColorFilter(0xFFFF0000);
                                                  IMAGE0_flag = 1;
                                                  Toast.makeText(MainActivity.this, "侧翻，方向->", Toast.LENGTH_SHORT).show(); //在当前activity显示内容为"hello"的短时间弹窗
                                                  /*******JSON格式发送负载******/
                                                  JSONObject root = new JSONObject();
                                                  try {

                                                      root.put("Positive",1);//对其添加一个数据
                                                      root.put("Negative",0);//对其添加一个数据
                                                  } catch (JSONException e) {
                                                      e.printStackTrace();
                                                  }
                                                  publishmessageplus(mqtt_pub_topic,root.toString());

                                              } else {
                                                  ID_IMAGE_0.setColorFilter(0x00000000);
                                                  IMAGE0_flag = 0;
                                                  Toast.makeText(MainActivity.this, "侧翻，方向<-", Toast.LENGTH_SHORT).show(); //在当前activity显示内容为"hello"的短时间弹窗

                                                    /*******JSON格式发送负载******/
                                                  JSONObject root = new JSONObject();
                                                  try {

                                                      root.put("Positive",0);//对其添加一个数据
                                                      root.put("Negative",1);//对其添加一个数据
                                                  } catch (JSONException e) {
                                                      e.printStackTrace();
                                                  }
                                                  publishmessageplus(mqtt_pub_topic,root.toString());



                                              }
                                          }
                                      }
        );//IMAGE0单击方法


        /***********SeekBar数值改变方法*********/

        ID_SeekBar_0.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener()

                                                {



                                                    public void onProgressChanged (SeekBar seekBar, final int progress,
                                                                                   boolean fromUser){
                                                        // TODO Auto-generated method stub
                                                        if (ID_SeekBar_0.getId() == R.id.ID_SeekBar_0) {

                                                            Toast.makeText(MainActivity.this, "seekBar当前位置"+progress, Toast.LENGTH_SHORT).show(); //在当前activity显示内容为""的短时间弹窗
/*****************线程阻塞延的方法*******************/
//                                                            try {
//                                                                Thread.sleep(250);
//                                                            } catch (InterruptedException e) {
//                                                                e.printStackTrace();
//                                                            }

                                                            /********Time方法延时(待测试嵌入式压力)*************/
                                                            TimerTask task = new TimerTask() {
                                                                @Override
                                                                public void run() {
                                                                    /**
                                                                     *要执行的操作
                                                                     */
                                                                    /*******JSON格式发送负载******/
                                                            JSONObject root = new JSONObject();
                                                            try {

                                                                root.put("Speed",((double)progress/100)*250);//对其添加一个数据

                                                            } catch (JSONException e) {
                                                                e.printStackTrace();
                                                            }
                                                            publishmessageplus(mqtt_pub_topic,root.toString());



                                                                }
                                                            };
                                                            Timer timer = new Timer();
                                                            timer.schedule(task, 250);//3秒后执行TimeTask的run方法


//                                                            /*******JSON格式发送负载******/
//                                                            JSONObject root = new JSONObject();
//                                                            try {
//
//                                                                root.put("Speed",((double)progress/100)*250);//对其添加一个数据
//
//                                                            } catch (JSONException e) {
//                                                                e.printStackTrace();
//                                                            }
//                                                            publishmessageplus(mqtt_pub_topic,root.toString());


                                                        } else {
                                                            Toast.makeText(MainActivity.this, "seekBar当前位置" + progress, Toast.LENGTH_SHORT).show(); //在当前activity显示内容为"hello"的短时间弹窗
                                                        }
                                                    }


                                                    public void onStartTrackingTouch (SeekBar seekBar){
                                                        // TODO Auto-generated method stub

                                                    }

                                                    @Override
                                                    public void onStopTrackingTouch(SeekBar seekBar) {

                                                    }

                                                }
        );

/***************************************MQTT连接**********************************************/
        Mqtt_init();
        startReconnect();

/***********mqtt连接句柄******************/

        handler = new Handler(){
            @SuppressLint("HandlerLeak")
            public void handleMessage(Message  msg) {
                super.handleMessage(msg);
                switch (msg.what){
                    case 1: //开机校验更新回传
                        Toast.makeText(MainActivity.this,"验证更新" ,Toast.LENGTH_SHORT).show();
                        break;
                    case 2:  // 反馈回传
                        Toast.makeText(MainActivity.this,"验证反馈" ,Toast.LENGTH_SHORT).show();
                        break;
                    case 3:  //MQTT 收到消息回传   UTF8Buffer msg=new UTF8Buffer(object.toString());
                        break;
                    case 30:  //连接失败
                        Toast.makeText(MainActivity.this,"连接失败" ,Toast.LENGTH_SHORT).show();
                        break;
                    case 31:   //连接成功
                        Toast.makeText(MainActivity.this,"连接成功" ,Toast.LENGTH_SHORT).show();
                        try {
                            client.subscribe(mqtt_sub_topic,0);
                        } catch (MqttException e) {
                            e.printStackTrace();
                        }
                        break;
                    default:
                        break;
                }
            }
        };


    } //onCreate


        /***************************调用函数*******************************/
        /***ui初始化****/
        private void ui_init() {

            ID_IMAGE_0 = findViewById(R.id.ID_IMAGE_0);//寻找xml中真正的id
            ID_SeekBar_0 = findViewById(R.id.ID_SeekBar_0);//寻找xml中真正的id

        } //UI INIT


    /***MQTT初始化****/
    private void Mqtt_init()
    {
        try {
            //host为主机名，test为clientid即连接MQTT的客户端ID，一般以客户端唯一标识符表示，MemoryPersistence设置clientid的保存形式，默认为以内存保存
            client = new MqttClient(host, mqtt_id,
                    new MemoryPersistence());
            //MQTT的连接设置
            options = new MqttConnectOptions();
            //设置是否清空session,这里如果设置为false表示服务器会保留客户端的连接记录，这里设置为true表示每次连接到服务器都以新的身份连接
            options.setCleanSession(false);
            //设置连接的用户名
            options.setUserName(userName);
            //设置连接的密码
            options.setPassword(passWord.toCharArray());
            // 设置超时时间 单位为秒
            options.setConnectionTimeout(10);
            // 设置会话心跳时间 单位为秒 服务器会每隔1.5*20秒的时间向客户端发送个消息判断客户端是否在线，但这个方法并没有重连的机制
            options.setKeepAliveInterval(20);
            //设置回调
            client.setCallback(new MqttCallback() {
                @Override
                public void connectionLost(Throwable cause) {
                    //连接丢失后，一般在这里面进行重连
                    System.out.println("connectionLost----------");
                    //startReconnect();
                }
                @Override
                public void deliveryComplete(IMqttDeliveryToken token) {
                    //publish后会执行到这里
                    System.out.println("deliveryComplete---------"
                            + token.isComplete());
                }
                @Override
                public void messageArrived(String topicName, MqttMessage message)
                        throws Exception {
                    //subscribe后得到的消息会执行到这里面
                    System.out.println("messageArrived----------");
                    Message msg = new Message();
                    msg.what = 3;   //收到消息标志位
                    msg.obj = topicName + "---" + message.toString();
                    handler.sendMessage(msg);    // hander 回传
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }
    }  //MQTT INIT

    /***MQTTConnect报文****/
    private void Mqtt_connect() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    if(!(client.isConnected()) )  //如果还未连接
                    {
                        client.connect(options);
                        Message msg = new Message();
                        msg.what = 31;
                        handler.sendMessage(msg);
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                    Message msg = new Message();
                    msg.what = 30;
                    handler.sendMessage(msg);
                }
            }
        }).start();
    } //MQTT CONNECT

    /***MQTT重连机制****/
    private void startReconnect() {
        scheduler = Executors.newSingleThreadScheduledExecutor();
        scheduler.scheduleAtFixedRate(new Runnable() {
            @Override
            public void run() {
                if (!client.isConnected()) {
                    Mqtt_connect();
                }
            }
        }, 0 * 1000, 10 * 1000, TimeUnit.MILLISECONDS);
    }//MQTT RECONNECT

    /***MQTTPublish报文****/
    private void publishmessageplus(String topic,String message2)
    {
        if (client == null || !client.isConnected()) {
            return;
        }
        MqttMessage message = new MqttMessage();
        message.setQos(0);// 消息质量
        message.setRetained(true);// 断开链接是否保存消息，true保存
        message.setPayload(message2.getBytes());
        try {
            client.publish(topic,message);
        } catch (MqttException e) {

            e.printStackTrace();
        }
    }//MQTT PUBLISH




}//MainActivity 类