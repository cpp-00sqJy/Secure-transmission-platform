-- 01 ������ռ�
-- ע���ռ��·�� ����ʵ�ʰ�װ�������е���

CREATE TABLESPACE ts_seckey_admin 
    LOGGING  
    DATAFILE '/u01/app/oracle/oradata/orcl/ts_seckeymng_admin.dbf' SIZE 50M 
    EXTENT MANAGEMENT LOCAL;
    

CREATE TABLESPACE ts_seckey_op
    LOGGING 
    DATAFILE '/u01/app/oracle/oradata/orcl/ts_seckeymng_op.dbf' SIZE 200M
    EXTENT MANAGEMENT LOCAL;
    
ALTER DATABASE DATAFILE '/u01/app/oracle/oradata/orcl/ts_seckeymng_admin.dbf' AUTOEXTEND ON NEXT   50M MAXSIZE UNLIMITED;
ALTER DATABASE DATAFILE '/u01/app/oracle/oradata/orcl/ts_seckeymng_op.dbf' AUTOEXTEND ON NEXT  200M MAXSIZE UNLIMITED; 

commit;


-- 02 �������� (�����û�)
CREATE USER SECMNG  PROFILE DEFAULT 
    IDENTIFIED BY SECMNG DEFAULT TABLESPACE USERS
    ACCOUNT UNLOCK;

-- ��Դ�͵�¼Ȩ��
GRANT resource TO SECMNG;
GRANT create session TO SECMNG;


-- ���� ������Ϣ�� --��� ���� ���� ��Ȩ�� ״̬(0����  1������)
CREATE TABLE SECMNG.SECNODE(
	id     				char(4) PRIMARY KEY, 
	name     			VARCHAR2(128) NOT NULL,
 	nodedesc      		VARCHAR2(512),
 	createtime			date,
 	authcode			NUMBER(12),
 	state     			NUMBER(4)
)TABLESPACE ts_seckey_admin;

INSERT INTO SECMNG.secnode VALUES('0001', '��������', '�������ڽ�23��', '15-7��-15', 1, 0);
INSERT INTO SECMNG.secnode VALUES('1111', '�㶫������1111', '������ӽ���', '15-7��-15', 1111, 0);
commit;

-- ���� ������Կ��, �ͻ������� ������������ ��Կ�� ��Կ����ʱ�� ��Կ״̬ 
CREATE TABLE SECMNG.SECKEYINFO(
	clientid          	char(4) constraint secmng_seckeynode_clientid_fk references SECMNG.SECNODE(id),
	serverid          	char(4) constraint secmng_seckeynode_serverid_fk references SECMNG.SECNODE(id),
	keyid            	NUMBER(9) PRIMARY KEY, 
	createtime			date,
	state				NUMBER(4),
	seckey				VARCHAR2(512)
)TABLESPACE ts_seckey_admin;

--�������� ���µı�ռ���
CREATE  INDEX SECMNG.IX_SECKEYINFO_clientid ON SECMNG.SECKEYINFO(clientid) TABLESPACE ts_seckey_admin;

commit;


-- keysn���к�
CREATE TABLE SECMNG.KEYSN(
	ikeysn 		number(12)  PRIMARY KEY
) TABLESPACE ts_seckey_admin;

INSERT INTO SECMNG.KEYSN(ikeysn) VALUES (1);
commit;


CREATE TABLE SECMNG.SRVCFG(
	key				VARCHAR2(64),
	valude			VARCHAR2(128)
)TABLESPACE ts_seckey_admin;
	
-- ���� ������Ϣ�� ������ ����ʱ�� �����¼� �������� ��ʲô�� ��ʲôʱ�� ����ʲô�£�
CREATE TABLE SECMNG.TRAN(
	iID					Number(12) PRIMARY KEY,
 	tran_operator     	NUMBER,
 	trantime			date,
 	tranid          	NUMBER(4),
 	trandesc      		VARCHAR2(512)	
)TABLESPACE ts_seckey_op;


-- ��������
CREATE INDEX SECMNG.IX_TRAN_tran_operator ON SECMNG.TRAN(tran_operator) TABLESPACE ts_seckey_op;

-- ���������� �����ֶ�
CREATE SEQUENCE SECMNG.SEQiID INCREMENT BY 1 START WITH 1 
    MAXVALUE 1.0E28 MINVALUE 1 NOCYCLE 
    CACHE 20 NOORDER;
    
CREATE OR REPLACE TRIGGER SECMNG.SEQiID
  BEFORE INSERT ON SECMNG.TRAN
    for each row
begin
    select SECMNG.SEQiID.nextval into :new.iID from dual;
end;
/


INSERT INTO SECMNG.TRAN(tran_operator, trantime, tranid, trandesc) VALUES(1, '15-7��-15', 1, '���������ܽڵ�-��������');

commit;

------ֹͣ-----
-- 04�������û����� 	ͨ�� SECMNGUSER1 ���������ݿ�, �ù����ն�ϵͳ��   
CREATE USER "SECMNGADMIN"  PROFILE "DEFAULT" IDENTIFIED BY "123456" DEFAULT TABLESPACE "USERS" ACCOUNT UNLOCK;

GRANT "CONNECT" TO "SECMNGADMIN";
GRANT SELECT ANY TABLE TO "SECMNGADMIN";

GRANT resource TO SECMNGADMIN;
GRANT create session TO SECMNGADMIN;


GRANT DELETE ON SECMNG.SRVCFG TO "SECMNGADMIN";
GRANT INSERT ON SECMNG.SRVCFG TO "SECMNGADMIN";
GRANT UPDATE ON SECMNG.SRVCFG TO "SECMNGADMIN";


GRANT DELETE ON SECMNG.SECNODE TO "SECMNGADMIN";
GRANT INSERT ON SECMNG.SECNODE TO "SECMNGADMIN";
GRANT UPDATE ON SECMNG.SECNODE TO "SECMNGADMIN";

GRANT DELETE ON SECMNG.SECKEYINFO TO "SECMNGADMIN";
GRANT INSERT ON SECMNG.SECKEYINFO TO "SECMNGADMIN";
GRANT UPDATE ON SECMNG.SECKEYINFO TO "SECMNGADMIN";

GRANT DELETE ON SECMNG.TRAN TO "SECMNGADMIN";
GRANT INSERT ON SECMNG.TRAN TO "SECMNGADMIN";
GRANT UPDATE ON SECMNG.TRAN TO "SECMNGADMIN";

commit;

    
 
-- Insert Into SECMNG.SECKEYINFO(clientid, serverid, keyid, createtime, state, seckey)
-- values ('1111', '0001', 1, to_date('2015-07-14 21:09:09', 'yyyy-mm-dd hh24:mi:ss'), 0, 'zzz')  ;
