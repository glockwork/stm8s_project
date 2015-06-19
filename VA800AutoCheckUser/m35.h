#ifndef __VA800_M35_H
#define __VA800_M35_H

/*AT command for Connect to module start*/
#define AT	                "AT\r\n"
#define ATV1	                "ATV1\r\n"
#define ATE1	                "ATE1\r\n"
#define AT_CMEE_SET_2	        "AT+CMEE=2\r\n"
#define AT_IPR_GET	        "AT+IPR?\r\n"
#define AT_IPR_SET_115200	"AT+IPR=115200\r\n"
#define ATI	                "ATI\r\n"               //��ȡģ����Ϣ
#define AT_GSN	                "AT+GSN\r\n"
#define AT_CPIN_GET	        "AT+CPIN?\r\n"
#define AT_CIMI	                "AT+CIMI\r\n"
#define AT_QCCID	        "AT+QCCID\r\n"
#define AT_CSQ_GET	        "AT+CSQ\r\n"            //��ȡģ���ź�ǿ��
#define AT_CREG_GET	        "AT+CREG?\r\n"
#define AT_CGREG_GET	        "AT+CGREG?\r\n"
#define AT_COPS_GET	        "AT+COPS?\r\n"
/*AT command for Connect to module end*/

/*AT command for TCP/UDP start*/
#define AT_CPIN_GET	        "AT+CPIN?\r\n"
#define AT_CSQ_GET	        "AT+CSQ\r\n"
#define AT_CREG_GET	        "AT+CREG?\r\n"
#define AT_CGREG_GET	        "AT+CGREG?\r\n"
/*����Ϊ����ģʽ��׼��*/

#define AT_QIMODE_SET_0	        "AT+QIMODE=0\r\n"
#define AT_QICSGP_SET_1_CMNET	"AT+QICSGP=1,\"CMNET\"\r\n"
#define AT_QIREGAPP	        "AT+QIREGAPP\r\n"
#define AT_QICSGP_GET	        "AT+QICSGP?\r\n"
#define AT_QIACT	        "AT+QIACT\r\n"
#define AT_QILOCIP	        "AT+QILOCIP\r\n"        //��ȡģ�鱾��IP

#define ATV1	                "ATV1\r\n"
#define AT_QIDEACT              "AT+QIDEACT\r\n"
#define AT_QIHEAD_SET_1	        "AT+QIHEAD=1\r\n"
#define AT_QIDNSIP_SET_0	"AT+QIDNSIP=0\r\n"
#define MODE                    "TCP"    
#define IP                      "113.88.218.178"
#define PORT                    "20002"
#define AT_QIOPEN(MODE,IP,PORT) "AT+QIOPEN=\""##MODE"\",\""##IP"\",\""##PORT"\"\r\n"

/*����������ӷ�����IP��PORT*/

static uint8_t *ConnectModule[] = {
  AT, 
  ATV1,                 //������Ӧ�������ݸ�ʽ����Ϣ���ؽ��<CR><LF><text><CR><LF>
                        //                    ���ַ��ͽ����<CR><LF>verbose code><CR><LF>
  ATE1,                 /* set ATE1 to enable echo mode */
  AT_CMEE_SET_2,        /* use AT+CMEE=2 to enable result code and use verbose values */
  AT_IPR_SET_115200,
//  ATI,                /* Set ATI to get module information of Manufacturer ID, Device modle and Firmware version */
//  AT_GSN,               /* use AT+GSN to query the IMEI of module */
//  AT_CPIN_GET,          /* use AT+CPIN? to query the SIM card status : SIM card inserted or not, locked or unlocked */
//  AT_CIMI,              /* use AT+CIMI to query the IMSI of SIM card */
//  AT_QCCID,               /* use AT+QCCID to query ICCID number of SIM card */
//  AT_CSQ_GET,
//  AT_CREG_GET,
//  AT_CGREG_GET,
//  AT_COPS_GET
  AT_QIMODE_SET_0,              //TCPIP����Ϊ��͸��ģʽ
  AT_QICSGP_SET_1_CMNET,        //����GPRSģʽ���ӣ�����㣺CMNET
  AT_QIREGAPP,                  //�����������õĽ����APN���û���������Ϊ��
//  AT_QICSGP_GET,                //��ȡ����ģʽ����
//  AT_QIACT,                     //�����ƶ����������� GPRS/CSD �������ӣ�
//  AT_QILOCIP,                 //��ȡģ�鱾��IP
  
//  ATV1,
//  AT_QIDEACT,                   //�ر� GPRS/CSD PDP ����
  AT_QIHEAD_SET_1,              //���ý�������ʱ��ʾ IP ͷ
  AT_QIDNSIP_SET_0              //����ʹ�� IP ��ַ����, 1:��������
};

#define AT_QICLOSE              "AT+QICLOSE"    //�ر�TCPIP����
#define ATQISEND	        "AT+QISEND\r\n" //������������
/*AT command for TCP/UDP end*/

#endif