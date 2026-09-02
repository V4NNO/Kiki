/*
 * tlshost.exe - pseudocod decompilat cu Ghidra MCP
 * Data analizei: 2026-09-01
 *
 * IMPORTANT:
 * - Acesta nu este codul-sursa original si nu este garantat compilabil.
 * - Tipurile undefined*, numele FUN_/DAT_/LAB_ si prototipurile sunt cele
 *   produse de decompilatorul Ghidra.
 * - Sunt incluse numai functii existente in programul analizat; nu au fost
 *   inventate functii si nu au fost rescrise implementari.
 * - Selectia acopera pornirea, profilul tlshost, serviciul Windows,
 *   sesiunile WTS/procesele utilizator, transportul si traseul video.
 */

// =============================================================================
// Ghidra: entry @ 0x14000e890
// =============================================================================
void entry(void)

{
  __security_init_cookie();
  FUN_1402985d4();
  return;
}

// =============================================================================
// Ghidra: FUN_1402985d4 @ 0x1402985d4
// =============================================================================
int FUN_1402985d4(void)

{
  code *pcVar1;
  bool bVar2;
  char cVar3;
  undefined1 uVar4;
  undefined2 uVar5;
  int iVar6;
  longlong *plVar7;
  undefined8 uVar8;
  
  cVar3 = __scrt_initialize_crt(1);
  if (cVar3 == '\0') {
                    /* WARNING: Subroutine does not return */
    __scrt_fastfail(7);
  }
  bVar2 = false;
  uVar4 = __scrt_acquire_startup_lock();
  if (DAT_140540040 == 1) {
                    /* WARNING: Subroutine does not return */
    __scrt_fastfail(7);
  }
  if (DAT_140540040 == 0) {
    DAT_140540040 = 1;
    iVar6 = _initterm_e(&DAT_14033ed48,&DAT_14033f188);
    if (iVar6 != 0) {
      return 0xff;
    }
    _initterm(&DAT_14033d000,&DAT_14033e908);
    DAT_140540040 = 2;
  }
  else {
    bVar2 = true;
  }
  __scrt_release_startup_lock(uVar4);
  plVar7 = (longlong *)thunk_FUN_1402996b8();
  if ((*plVar7 != 0) && (cVar3 = __scrt_is_nonwritable_in_current_image(plVar7), cVar3 != '\0')) {
    pcVar1 = (code *)*plVar7;
    _guard_check_icall(pcVar1);
    (*pcVar1)(0,2);
  }
  plVar7 = (longlong *)thunk_FUN_1402996c4();
  if ((*plVar7 != 0) && (cVar3 = __scrt_is_nonwritable_in_current_image(plVar7), cVar3 != '\0')) {
    _register_thread_local_exe_atexit_callback(*plVar7);
  }
  uVar5 = __scrt_get_show_window_mode();
  uVar8 = _get_narrow_winmain_command_line();
  iVar6 = thunk_FUN_1402dab90(&IMAGE_DOS_HEADER_140000000,0,uVar8,uVar5);
  cVar3 = __scrt_is_managed_app();
  if (cVar3 != '\0') {
    if (!bVar2) {
      _cexit();
    }
    __scrt_uninitialize_crt(1,0);
    return iVar6;
  }
                    /* WARNING: Subroutine does not return */
  exit(iVar6);
}

// =============================================================================
// Ghidra: FUN_140085290 @ 0x140085290
// =============================================================================
int FUN_140085290(int param_1,undefined8 *param_2)

{
  bool bVar1;
  code *this;
  char cVar2;
  int iVar3;
  undefined4 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  QChar *pQVar7;
  QString *pQVar8;
  QByteArray *this_00;
  char *pcVar9;
  longlong lVar10;
  QFileInfo *this_01;
  longlong lVar11;
  bool bVar12;
  int local_res8 [2];
  QTypedArrayData<unsigned_short> *local_res10;
  QTypedArrayData<unsigned_short> *local_res18;
  QTypedArrayData<unsigned_short> *local_res20;
  undefined1 local_120 [24];
  undefined1 local_108 [64];
  undefined1 local_c8 [16];
  undefined1 local_b8 [136];
  
  bVar12 = false;
  local_res10 = (QTypedArrayData<unsigned_short> *)((ulonglong)local_res10 & 0xffffffff00000000);
  local_res8[0] = param_1;
  local_res10 = QString::fromAscii_helper("KickidlerGrabber",0x10);
  QCoreApplication::setApplicationName((QString *)&local_res10);
  QString::~QString((QString *)&local_res10);
  local_res18 = (QTypedArrayData<unsigned_short> *)&local_res10;
  uVar5 = QFile::decodeName((char *)&local_res10);
  uVar6 = thunk_FUN_1400766c0();
  thunk_FUN_14010c8b0(uVar6,1,uVar5);
  uVar5 = thunk_FUN_140084f60(&local_res10,0);
  thunk_FUN_140184d00(uVar5);
  QString::~QString((QString *)&local_res10);
  lVar11 = -1;
  if (local_res8[0] < 2) {
LAB_140085392:
    bVar1 = false;
  }
  else {
    local_res18 = QString::fromAscii_helper("-utilize-invite",0xf);
    bVar12 = true;
    local_res10 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_res10._4_4_,1);
    pcVar9 = (char *)param_2[1];
    iVar3 = *(int *)(local_res18 + 4);
    pQVar7 = QString::constData((QString *)&local_res18);
    iVar3 = QString::compare_helper(pQVar7,iVar3,pcVar9,-1,1);
    if (iVar3 != 0) goto LAB_140085392;
    bVar1 = true;
  }
  if (bVar12) {
    QString::~QString((QString *)&local_res18);
  }
  bVar12 = false;
  if (bVar1) {
    thunk_FUN_140095e40(local_120,1,local_res8,param_2);
    iVar3 = QCoreApplication::exec();
    thunk_FUN_1400968e0(local_120);
    return iVar3;
  }
  if (local_res8[0] == 3) {
    local_res18 = QString::fromAscii_helper("-encrypt",8);
    bVar12 = true;
    local_res10 = (QTypedArrayData<unsigned_short> *)
                  ((ulonglong)local_res10 & 0xffffffff00000000 | 2);
    pcVar9 = (char *)param_2[1];
    iVar3 = *(int *)(local_res18 + 4);
    pQVar7 = QString::constData((QString *)&local_res18);
    iVar3 = QString::compare_helper(pQVar7,iVar3,pcVar9,-1,1);
    if (iVar3 != 0) goto LAB_140085439;
    bVar1 = true;
  }
  else {
LAB_140085439:
    bVar1 = false;
  }
  if (bVar12) {
    QString::~QString((QString *)&local_res18);
  }
  bVar12 = false;
  if (bVar1) {
    pcVar9 = (char *)param_2[2];
    if (pcVar9 != (char *)0x0) {
      do {
        lVar11 = lVar11 + 1;
      } while (pcVar9[lVar11] != '\0');
    }
    local_res10 = QString::fromAscii_helper(pcVar9,(int)lVar11);
    pQVar8 = (QString *)thunk_FUN_14017dde0(&local_res20,&local_res10);
    this = cout_exref;
    this_00 = (QByteArray *)QString::toLocal8Bit(pQVar8);
    pcVar9 = QByteArray::data(this_00);
    thunk_FUN_140042af0(this,pcVar9);
    QByteArray::~QByteArray((QByteArray *)&local_res18);
    std::basic_ostream<char,std::char_traits<char>_>::operator<<
              ((basic_ostream<char,std::char_traits<char>_> *)this,thunk_FUN_140083cd0);
    QString::~QString((QString *)&local_res20);
    QString::~QString((QString *)&local_res10);
    return 0;
  }
  if (local_res8[0] < 4) {
LAB_140085532:
    bVar1 = false;
  }
  else {
    local_res18 = QString::fromAscii_helper("report",6);
    bVar12 = true;
    local_res10 = (QTypedArrayData<unsigned_short> *)
                  ((ulonglong)local_res10 & 0xffffffff00000000 | 4);
    cVar2 = thunk_FUN_1400841e0(param_2[1],&local_res18);
    if (cVar2 == '\0') goto LAB_140085532;
    bVar1 = true;
  }
  if (bVar12) {
    QString::~QString((QString *)&local_res18);
  }
  if (!bVar1) {
    uVar5 = thunk_FUN_140084f60(&local_res20,8);
    uVar5 = thunk_FUN_140060f80(&local_res18,uVar5,&DAT_14034d404);
    uVar5 = thunk_FUN_140060f80(&local_res10,uVar5,"grabber");
    thunk_FUN_14018d840(uVar5,0xa00000,10);
    QString::~QString((QString *)&local_res18);
    QString::~QString((QString *)&local_res20);
    if (0 < local_res8[0]) {
      pQVar8 = (QString *)QString::fromLocal8Bit((char *)&local_res20,(int)*param_2);
      this_01 = (QFileInfo *)QFileInfo::QFileInfo((QFileInfo *)&local_res18,pQVar8);
      uVar5 = QFileInfo::fileName(this_01);
      thunk_FUN_140091290(uVar5);
      QFileInfo::~QFileInfo((QFileInfo *)&local_res18);
      QString::~QString((QString *)&local_res20);
    }
    thunk_FUN_14008c190(local_c8,local_res8[0],param_2);
    iVar3 = thunk_FUN_14010e7f0(local_b8);
    thunk_FUN_14008c6a0(local_c8);
    return iVar3;
  }
  if (local_res8[0] < 5) {
    pcVar9 = "";
  }
  else {
    pcVar9 = (char *)param_2[4];
    if (pcVar9 == (char *)0x0) {
      lVar10 = 0xffffffff;
      goto LAB_140085579;
    }
  }
  lVar10 = -1;
  do {
    lVar10 = lVar10 + 1;
  } while (pcVar9[lVar10] != '\0');
LAB_140085579:
  local_res20 = QString::fromAscii_helper(pcVar9,(int)lVar10);
  pcVar9 = (char *)param_2[3];
  if (pcVar9 == (char *)0x0) {
    lVar10 = 0xffffffff;
  }
  else {
    lVar10 = -1;
    do {
      lVar10 = lVar10 + 1;
    } while (pcVar9[lVar10] != '\0');
  }
  local_res18 = QString::fromAscii_helper(pcVar9,(int)lVar10);
  pcVar9 = (char *)param_2[2];
  if (pcVar9 != (char *)0x0) {
    do {
      lVar11 = lVar11 + 1;
    } while (pcVar9[lVar11] != '\0');
  }
  local_res10 = QString::fromAscii_helper(pcVar9,(int)lVar11);
  uVar4 = thunk_FUN_14011d1b0(&local_res10);
  thunk_FUN_140119220(local_108,1,uVar4,&local_res18,&local_res20,local_res8,param_2);
  QString::~QString((QString *)&local_res10);
  QString::~QString((QString *)&local_res18);
  QString::~QString((QString *)&local_res20);
  iVar3 = QCoreApplication::exec();
  thunk_FUN_140119d40(local_108);
  return iVar3;
}

// =============================================================================
// Ghidra: FUN_140089540 @ 0x140089540
// =============================================================================
undefined8 * FUN_140089540(undefined8 *param_1,QString *param_2)

{
  int iVar1;
  QString *pQVar2;
  QTypedArrayData<unsigned_short> *pQVar3;
  QString local_res18 [8];
  
  pQVar2 = (QString *)QString::fromLatin1((char *)local_res18,0x4034dbb8);
  iVar1 = QString::compare(param_2,pQVar2,0);
  QString::~QString(local_res18);
  if (iVar1 != 0) {
    pQVar2 = (QString *)QString::fromLatin1((char *)local_res18,0x4034dbc8);
    iVar1 = QString::compare(param_2,pQVar2,0);
    QString::~QString(local_res18);
    if (iVar1 != 0) {
      pQVar2 = (QString *)QString::fromLatin1((char *)local_res18,0x4034dbd8);
      iVar1 = QString::compare(param_2,pQVar2,0);
      QString::~QString(local_res18);
      if (iVar1 != 0) {
        pQVar2 = (QString *)QString::fromLatin1((char *)local_res18,0x4034dc60);
        iVar1 = QString::compare(param_2,pQVar2,0);
        QString::~QString(local_res18);
        if (iVar1 != 0) {
          pQVar2 = (QString *)QString::fromLatin1((char *)local_res18,0x4034dc70);
          iVar1 = QString::compare(param_2,pQVar2,0);
          QString::~QString(local_res18);
          if (iVar1 != 0) {
            pQVar2 = (QString *)QString::fromLatin1((char *)local_res18,0x4034dc80);
            iVar1 = QString::compare(param_2,pQVar2,0);
            QString::~QString(local_res18);
            if (iVar1 != 0) {
              pQVar2 = (QString *)QString::fromLatin1((char *)local_res18,0x4034dcc8);
              iVar1 = QString::compare(param_2,pQVar2,0);
              QString::~QString(local_res18);
              if (iVar1 != 0) {
                pQVar2 = (QString *)QString::fromLatin1((char *)local_res18,0x4034dcd8);
                iVar1 = QString::compare(param_2,pQVar2,0);
                QString::~QString(local_res18);
                if (iVar1 != 0) {
                  pQVar2 = (QString *)QString::fromLatin1((char *)local_res18,0x4034dcf0);
                  iVar1 = QString::compare(param_2,pQVar2,0);
                  QString::~QString(local_res18);
                  if (iVar1 != 0) {
                    pQVar2 = (QString *)QString::fromLatin1((char *)local_res18,0x4034dd60);
                    iVar1 = QString::compare(param_2,pQVar2,0);
                    QString::~QString(local_res18);
                    if (iVar1 != 0) {
                      pQVar2 = (QString *)QString::fromLatin1((char *)local_res18,0x4034dd70);
                      iVar1 = QString::compare(param_2,pQVar2,0);
                      QString::~QString(local_res18);
                      if (iVar1 != 0) {
                        pQVar2 = (QString *)QString::fromLatin1((char *)local_res18,0x4034dd88);
                        iVar1 = QString::compare(param_2,pQVar2,0);
                        QString::~QString(local_res18);
                        if (iVar1 != 0) {
                          pQVar3 = QString::fromAscii_helper("grabber",7);
                          *param_1 = pQVar3;
                          pQVar3 = QString::fromAscii_helper("grabberAgent",0xc);
                          param_1[1] = pQVar3;
                          pQVar3 = QString::fromAscii_helper("grabberSubAgent",0xf);
                          param_1[2] = pQVar3;
                          pQVar3 = QString::fromAscii_helper("ngs",3);
                          param_1[3] = pQVar3;
                          pQVar3 = QString::fromAscii_helper("Tele Link Soft Helper Service",0x1d);
                          param_1[4] = pQVar3;
                          goto LAB_140089a6c;
                        }
                      }
                    }
                    pQVar3 = QString::fromAscii_helper("grabber",7);
                    *param_1 = pQVar3;
                    pQVar3 = QString::fromAscii_helper("grabberAgent",0xc);
                    param_1[1] = pQVar3;
                    pQVar3 = QString::fromAscii_helper("grabberSubAgent",0xf);
                    param_1[2] = pQVar3;
                    pQVar3 = QString::fromAscii_helper("ngs",3);
                    param_1[3] = pQVar3;
                    pQVar3 = QString::fromAscii_helper("Tele Link Soft Helper Service",0x1d);
                    param_1[4] = pQVar3;
                    goto LAB_140089a6c;
                  }
                }
              }
              pQVar3 = QString::fromAscii_helper("tlshost",7);
              *param_1 = pQVar3;
              pQVar3 = QString::fromAscii_helper("tlsservice",10);
              param_1[1] = pQVar3;
              pQVar3 = QString::fromAscii_helper("tlssubservice",0xd);
              param_1[2] = pQVar3;
              pQVar3 = QString::fromAscii_helper("tls",3);
              param_1[3] = pQVar3;
              pQVar3 = QString::fromAscii_helper("Telemetry Service Host",0x16);
              param_1[4] = pQVar3;
              goto LAB_140089a6c;
            }
          }
        }
        pQVar3 = QString::fromAscii_helper("grabber2",8);
        *param_1 = pQVar3;
        pQVar3 = QString::fromAscii_helper("ngsAgent",8);
        param_1[1] = pQVar3;
        pQVar3 = QString::fromAscii_helper("ngsSubAgent",0xb);
        param_1[2] = pQVar3;
        pQVar3 = QString::fromAscii_helper("ngs",3);
        param_1[3] = pQVar3;
        pQVar3 = QString::fromAscii_helper("Tele Link Soft Helper Service",0x1d);
        param_1[4] = pQVar3;
        goto LAB_140089a6c;
      }
    }
  }
  pQVar3 = QString::fromAscii_helper("svchost",7);
  *param_1 = pQVar3;
  pQVar3 = QString::fromAscii_helper("WmiPrvSE",8);
  param_1[1] = pQVar3;
  pQVar3 = QString::fromAscii_helper("RuntimeBroker",0xd);
  param_1[2] = pQVar3;
  pQVar3 = QString::fromAscii_helper("WmiProviderSE",0xd);
  param_1[3] = pQVar3;
  pQVar3 = QString::fromAscii_helper("Windows Management Provider Service",0x23);
  param_1[4] = pQVar3;
LAB_140089a6c:
  QString::~QString(param_2);
  return param_1;
}

// =============================================================================
// Ghidra: FUN_140086b60 @ 0x140086b60
// =============================================================================
/* WARNING: Function: _alloca_probe replaced with injection: alloca_probe */
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_140086b60(undefined8 param_1)

{
  QHashData *pQVar1;
  ulong uVar2;
  undefined1 auVar3 [16];
  char cVar4;
  int iVar5;
  DWORD DVar6;
  BOOL BVar7;
  longlong *plVar8;
  undefined8 uVar9;
  QString *pQVar10;
  QFileInfo *this;
  undefined8 uVar11;
  QDir *this_00;
  basic_ostream<char,std::char_traits<char>_> *pbVar12;
  basic_ostream<char,struct_std::char_traits<char>_> *pbVar13;
  __uint64 _Var14;
  LPWSTR lpCommandLine;
  ushort *_Src;
  LPCWSTR lpCurrentDirectory;
  LPCWSTR lpApplicationName;
  QByteArray *this_01;
  char *pcVar15;
  longlong lVar16;
  QJsonDocument *this_02;
  QVariant *pQVar17;
  Node *pNVar18;
  undefined ***pppuVar19;
  ulonglong uVar20;
  ulonglong uVar21;
  basic_ios<char,std::char_traits<char>_> *this_03;
  longlong lVar22;
  longlong lVar23;
  uint uVar24;
  Node *pNVar25;
  uint uVar26;
  undefined1 auStackY_1b38 [32];
  Node *local_1ad0;
  code *local_1ac8;
  undefined ***local_1ac0;
  uint local_1ab8;
  uint local_1ab4;
  HANDLE local_1ab0;
  HANDLE local_1aa8;
  longlong local_1aa0;
  code *local_1a98;
  HANDLE local_1a90;
  QString local_1a88 [8];
  uint local_1a80;
  uint local_1a7c;
  Node *local_1a78;
  Node *local_1a70;
  Node *local_1a68;
  uint local_1a60;
  QTypedArrayData<unsigned_short> *local_1a58;
  QTypedArrayData<unsigned_short> *local_1a50;
  QString local_1a48 [8];
  QString local_1a40 [8];
  longlong local_1a38;
  undefined ***local_1a30;
  QByteArray local_1a28 [8];
  _PROCESS_INFORMATION local_1a20;
  uint *local_1a00;
  QFileInfo local_19f8 [8];
  QString local_19f0 [8];
  QString local_19e8 [8];
  QString local_19e0 [8];
  QDir local_19d8 [8];
  QString local_19d0 [8];
  QString local_19c8 [8];
  QByteArray local_19c0 [8];
  QString local_19b8 [8];
  undefined8 local_19b0;
  QVariant local_19a8 [16];
  QVariant local_1998 [16];
  _SECURITY_ATTRIBUTES local_1988;
  QString local_1970 [8];
  QString local_1968 [8];
  QString local_1960 [8];
  QString local_1958 [8];
  QString local_1950 [8];
  undefined1 local_1948 [16];
  undefined1 local_1938 [16];
  undefined1 local_1928 [16];
  undefined1 local_1918 [16];
  undefined1 local_1908 [16];
  undefined1 local_18f8 [16];
  undefined1 local_18e8 [16];
  undefined1 local_18d8 [16];
  undefined1 local_18c8 [16];
  undefined1 local_18b8 [16];
  undefined1 local_18a8 [16];
  QVariant local_1898 [16];
  QVariant local_1888 [16];
  QVariant local_1878 [16];
  undefined1 local_1868 [16];
  undefined1 local_1858 [16];
  undefined1 local_1848 [8];
  undefined8 local_1840;
  undefined1 local_1838 [8];
  undefined1 local_1830 [20];
  int iStack_181c;
  undefined *local_1818;
  undefined **local_1810;
  basic_ostream<char,std::char_traits<char>_> local_1808 [96];
  undefined8 local_17a8;
  undefined4 local_17a0;
  basic_ios<char,std::char_traits<char>_> local_1790 [100];
  int iStack_172c;
  undefined *local_1728;
  undefined **local_1720;
  basic_ostream<char,std::char_traits<char>_> local_1718 [96];
  undefined8 local_16b8;
  undefined4 local_16b0;
  basic_ios<char,std::char_traits<char>_> local_16a0 [100];
  int iStack_163c;
  undefined *local_1638;
  undefined **local_1630;
  basic_ostream<char,std::char_traits<char>_> local_1628 [96];
  undefined8 local_15c8;
  undefined4 local_15c0;
  basic_ios<char,std::char_traits<char>_> local_15b0 [100];
  int iStack_154c;
  undefined *local_1548;
  undefined **local_1540 [13];
  undefined8 local_14d8;
  undefined4 local_14d0;
  basic_ios<char,std::char_traits<char>_> local_14c0 [100];
  int iStack_145c;
  undefined *local_1458;
  undefined **local_1450 [13];
  undefined8 local_13e8;
  undefined4 local_13e0;
  basic_ios<char,std::char_traits<char>_> local_13d0 [104];
  _STARTUPINFOW local_1368;
  int iStack_12fc;
  undefined *local_12f8;
  basic_streambuf<char,std::char_traits<char>_> local_12f0 [128];
  basic_ios<char,std::char_traits<char>_> local_1270 [100];
  int iStack_120c;
  undefined *local_1208;
  basic_streambuf<char,std::char_traits<char>_> local_1200 [128];
  basic_ios<char,std::char_traits<char>_> local_1180 [104];
  ulonglong local_1118 [3];
  ulonglong local_1100;
  ulonglong local_10f8 [3];
  ulonglong local_10e0;
  ulonglong local_10d8 [3];
  ulonglong local_10c0;
  ulonglong local_10b8 [3];
  ulonglong local_10a0;
  undefined8 local_1098 [3];
  ulonglong local_1080;
  ulonglong local_1078 [3];
  ulonglong local_1060;
  undefined1 local_1058 [32];
  char local_1038 [4096];
  ulonglong local_38;
  undefined8 local_30;
  
  local_30 = 0x140086b7c;
  local_1840 = 0xfffffffffffffffe;
  local_38 = DAT_14050e9d0 ^ (ulonglong)auStackY_1b38;
  uVar21 = 0;
  local_1ad0 = (Node *)shared_null_exref;
  local_19b0 = param_1;
  iVar5 = WTSEnumerateSessionsW(0,0);
  if (iVar5 == 0) {
    uVar11 = thunk_FUN_14018d250(local_1938);
    cVar4 = thunk_FUN_14018ceb0(uVar11,40000);
    thunk_FUN_14018bd10(local_1938);
    if (cVar4 != '\0') {
      local_1818 = &DAT_14034ae60;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_1790);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_1818,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_1810,false);
      *(undefined ***)((longlong)&local_1818 + (longlong)*(int *)(local_1818 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_181c + (longlong)*(int *)(local_1818 + 4)) =
           *(int *)(local_1818 + 4) + -0x88;
      local_1ac0 = &local_1810;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_1810);
      local_1810 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_17a8 = 0;
      local_17a0 = 4;
      DVar6 = GetLastError();
      pbVar12 = (basic_ostream<char,std::char_traits<char>_> *)
                thunk_FUN_140042af0(&local_1818,"WTSEnumerateSessionsW failed with error ");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar12,DVar6);
      uVar11 = thunk_FUN_14007ce40(&local_1818,local_1118);
      uVar9 = thunk_FUN_14018d250(local_1928);
      thunk_FUN_14018c4e0(uVar9,40000,uVar11);
      thunk_FUN_14018bd10(local_1928);
      if (0xf < local_1100) {
        uVar21 = local_1118[0];
        if (0xfff < local_1100 + 1) {
          if ((local_1118[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar21 = *(ulonglong *)(local_1118[0] - 8);
          if (local_1118[0] <= uVar21) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_1118[0] - uVar21 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_1118[0] - uVar21) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002199(uVar21);
      }
      *(undefined ***)((longlong)&local_1818 + (longlong)*(int *)(local_1818 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_181c + (longlong)*(int *)(local_1818 + 4)) =
           *(int *)(local_1818 + 4) + -0x88;
      local_1810 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140065bc0(&local_1810);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_1810);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_1808);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_1790);
    }
  }
  else {
    if (local_1ab4 != 0) {
      do {
        if (*(uint *)(local_1a38 + 0x10 + uVar21 * 0x18) < 5) {
          uVar26 = *(uint *)(local_1a38 + uVar21 * 0x18);
          local_1ab8 = uVar26;
          iVar5 = WTSQuerySessionInformationW(0,uVar26);
          if (iVar5 == 0) {
            uVar11 = thunk_FUN_14018d250(local_1830);
            cVar4 = thunk_FUN_14018ceb0(uVar11,30000);
            thunk_FUN_14018bd10(local_1830);
            if (cVar4 != '\0') {
              local_1548 = &DAT_14034ae60;
              std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                        (local_14c0);
              std::basic_ostream<char,std::char_traits<char>_>::
              basic_ostream<char,std::char_traits<char>_>
                        ((basic_ostream<char,std::char_traits<char>_> *)&local_1548,
                         (basic_streambuf<char,std::char_traits<char>_> *)local_1540,false);
              *(undefined ***)((longlong)local_1540 + (longlong)*(int *)(local_1548 + 4) + -8) =
                   std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                   vftable;
              *(int *)((longlong)&iStack_154c + (longlong)*(int *)(local_1548 + 4)) =
                   *(int *)(local_1548 + 4) + -0x88;
              local_1ac0 = local_1540;
              std::basic_streambuf<char,std::char_traits<char>_>::
              basic_streambuf<char,std::char_traits<char>_>
                        ((basic_streambuf<char,std::char_traits<char>_> *)local_1540);
              local_1540[0] =
                   std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
              local_14d8 = 0;
              local_14d0 = 4;
              DVar6 = GetLastError();
              pbVar12 = (basic_ostream<char,std::char_traits<char>_> *)
                        thunk_FUN_140042af0(&local_1548,"WTSQuerySessionInformationW: ");
              std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar12,DVar6);
              uVar11 = thunk_FUN_14007ce40(&local_1548,local_1078);
              uVar9 = thunk_FUN_14018d250(local_1948);
              thunk_FUN_14018c4e0(uVar9,30000,uVar11);
              thunk_FUN_14018bd10(local_1948);
              if (0xf < local_1060) {
                uVar20 = local_1078[0];
                if (0xfff < local_1060 + 1) {
                  if ((local_1078[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                    _invalid_parameter_noinfo_noreturn();
                  }
                  uVar20 = *(ulonglong *)(local_1078[0] - 8);
                  if (local_1078[0] <= uVar20) {
                    /* WARNING: Subroutine does not return */
                    _invalid_parameter_noinfo_noreturn();
                  }
                  if (local_1078[0] - uVar20 < 8) {
                    /* WARNING: Subroutine does not return */
                    _invalid_parameter_noinfo_noreturn();
                  }
                  if (0x27 < local_1078[0] - uVar20) {
                    /* WARNING: Subroutine does not return */
                    _invalid_parameter_noinfo_noreturn();
                  }
                }
                FUN_140002199(uVar20);
              }
              thunk_FUN_14005cb60(local_14c0);
              std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                        (local_14c0);
            }
          }
          else {
            uVar24 = *local_1a00;
            WTSFreeMemory();
            if (((uVar24 & 0xfffffffa) == 0) && (uVar24 != 5)) {
              if (1 < *(uint *)(local_1ad0 + 0x10)) {
                thunk_FUN_140088e70(&local_1ad0);
              }
              uVar24 = *(uint *)(local_1ad0 + 0x24) ^ uVar26;
              local_1a80 = uVar24;
              plVar8 = (longlong *)thunk_FUN_140089360(&local_1ad0,&local_1ab8,uVar24);
              if ((QHashData *)*plVar8 == (QHashData *)local_1ad0) {
                if (*(int *)(local_1ad0 + 0x20) <= *(int *)(local_1ad0 + 0x14)) {
                  QHashData::rehash((QHashData *)local_1ad0,*(short *)(local_1ad0 + 0x1e) + 1);
                  uVar24 = *(uint *)(local_1ad0 + 0x24) ^ uVar26;
                  local_1a80 = uVar24;
                  plVar8 = (longlong *)thunk_FUN_140089360(&local_1ad0,&local_1ab8,uVar24);
                }
                local_1ac0 = QHashData::allocateNode((QHashData *)local_1ad0,8);
                if (local_1ac0 == (undefined ***)0x0) {
                  pppuVar19 = (undefined ***)0x0;
                }
                else {
                  *local_1ac0 = (undefined **)*plVar8;
                  *(uint *)(local_1ac0 + 1) = uVar24;
                  *(uint *)((longlong)local_1ac0 + 0xc) = uVar26;
                  pppuVar19 = local_1ac0;
                }
                *plVar8 = (longlong)pppuVar19;
                *(int *)(local_1ad0 + 0x14) = *(int *)(local_1ad0 + 0x14) + 1;
              }
            }
          }
        }
        uVar26 = (int)uVar21 + 1;
        uVar21 = (ulonglong)uVar26;
      } while (uVar26 < local_1ab4);
    }
    WTSFreeMemory(local_1a38);
  }
  pQVar10 = (QString *)QCoreApplication::applicationFilePath();
  this = (QFileInfo *)QFileInfo::QFileInfo(local_19f8,pQVar10);
  QFileInfo::fileName(this);
  QFileInfo::~QFileInfo(local_19f8);
  QString::~QString(local_19f0);
  uVar11 = QString::QString(local_19e8,local_1a40);
  uVar11 = thunk_FUN_140089540(local_1970,uVar11);
  thunk_FUN_14008b460(uVar11,local_1a88);
  QString::~QString(local_1950);
  QString::~QString(local_1958);
  QString::~QString(local_1960);
  QString::~QString(local_1968);
  QString::~QString(local_1970);
  local_1aa8 = (HANDLE)0x0;
  local_1a98 = shared_null_exref;
  pQVar10 = (QString *)QCoreApplication::applicationDirPath();
  this_00 = (QDir *)QDir::QDir(local_19d8,pQVar10);
  QDir::absolutePath(this_00);
  QDir::toNativeSeparators(local_1a48);
  QString::~QString(local_19e0);
  QDir::~QDir(local_19d8);
  QString::~QString(local_19d0);
  uVar11 = thunk_FUN_140061010(local_19c8,&DAT_14034deb4,local_1a88);
  thunk_FUN_140060f80(&local_1aa0,uVar11);
  QString::~QString(local_19c8);
  local_1988.nLength = 0x18;
  local_1988.bInheritHandle = 1;
  local_1988.lpSecurityDescriptor = (LPVOID)0x0;
  local_1ab0 = (HANDLE)0x0;
  local_1a90 = (HANDLE)0x0;
  local_1a78 = local_1ad0;
  pNVar18 = local_1ad0 + 0x10;
  if (1 < *(uint *)pNVar18 + 1) {
    LOCK();
    *(uint *)pNVar18 = *(uint *)pNVar18 + 1;
    UNLOCK();
  }
  if ((((byte)local_1ad0[0x28] & 1) == 0) && (1 < *(uint *)pNVar18)) {
    thunk_FUN_140088e70(&local_1a78);
  }
  plVar8 = *(longlong **)(local_1a78 + 8);
  iVar5 = *(int *)(local_1a78 + 0x20);
  while (pNVar18 = local_1a78, iVar5 != 0) {
    iVar5 = iVar5 + -1;
    if ((Node *)*plVar8 != local_1a78) {
      pNVar18 = (Node *)*plVar8;
      break;
    }
    plVar8 = plVar8 + 1;
  }
  local_1a60 = 1;
  pNVar25 = local_1a78;
  local_1a68 = local_1a78;
  do {
    local_1a70 = pNVar18;
    if (pNVar18 == pNVar25) break;
    uVar2 = *(ulong *)(pNVar18 + 0xc);
    if (local_1a60 != 0) {
      iVar5 = WTSQueryUserToken(uVar2,&local_1aa8);
      if (iVar5 == 0) {
        uVar11 = thunk_FUN_14018d250(local_1918);
        cVar4 = thunk_FUN_14018ceb0(uVar11,30000);
        thunk_FUN_14018bd10(local_1918);
        if (cVar4 != '\0') {
          local_1728 = &DAT_14034ae60;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_16a0);
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_1728,
                     (basic_streambuf<char,std::char_traits<char>_> *)&local_1720,false);
          *(undefined ***)((longlong)&local_1728 + (longlong)*(int *)(local_1728 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_172c + (longlong)*(int *)(local_1728 + 4)) =
               *(int *)(local_1728 + 4) + -0x88;
          local_1a30 = &local_1720;
          std::basic_streambuf<char,std::char_traits<char>_>::
          basic_streambuf<char,std::char_traits<char>_>
                    ((basic_streambuf<char,std::char_traits<char>_> *)&local_1720);
          local_1720 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                       vftable;
          local_16b8 = 0;
          local_16b0 = 4;
          DVar6 = GetLastError();
          pbVar12 = (basic_ostream<char,std::char_traits<char>_> *)
                    thunk_FUN_140042af0(&local_1728,"Failed to WTSQueryUserToken for sesssion ");
          pbVar13 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar12,uVar2);
          pbVar12 = (basic_ostream<char,std::char_traits<char>_> *)
                    thunk_FUN_140042af0(pbVar13,&DAT_140346ec4);
          std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar12,DVar6);
          thunk_FUN_14007ce40(&local_1728,local_10f8);
          uVar11 = thunk_FUN_14018d250(local_1908);
          thunk_FUN_14018c4e0(uVar11,30000);
          thunk_FUN_14018bd10(local_1908);
          if (0xf < local_10e0) {
            uVar21 = local_10f8[0];
            if (0xfff < local_10e0 + 1) {
              if ((local_10f8[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              uVar21 = *(ulonglong *)(local_10f8[0] - 8);
              if (local_10f8[0] <= uVar21) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (local_10f8[0] - uVar21 < 8) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (0x27 < local_10f8[0] - uVar21) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
            }
            FUN_140002199(uVar21);
          }
          *(undefined ***)((longlong)&local_1728 + (longlong)*(int *)(local_1728 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_172c + (longlong)*(int *)(local_1728 + 4)) =
               *(int *)(local_1728 + 4) + -0x88;
          local_1720 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                       vftable;
          thunk_FUN_140065bc0(&local_1720);
          std::basic_streambuf<char,std::char_traits<char>_>::
          ~basic_streambuf<char,std::char_traits<char>_>
                    ((basic_streambuf<char,std::char_traits<char>_> *)&local_1720);
          std::basic_ostream<char,std::char_traits<char>_>::
          ~basic_ostream<char,std::char_traits<char>_>(local_1718);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_16a0);
        }
      }
      else {
        BVar7 = CreatePipe(&local_1ab0,&local_1a90,&local_1988,0);
        if (BVar7 == 0) {
          uVar11 = thunk_FUN_14018d250(local_18f8);
          cVar4 = thunk_FUN_14018ceb0(uVar11,40000);
          thunk_FUN_14018bd10(local_18f8);
          if (cVar4 != '\0') {
            local_1638 = &DAT_14034ae60;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_15b0);
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1638,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1630,false);
            *(undefined ***)((longlong)&local_1638 + (longlong)*(int *)(local_1638 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_163c + (longlong)*(int *)(local_1638 + 4)) =
                 *(int *)(local_1638 + 4) + -0x88;
            local_1a30 = &local_1630;
            std::basic_streambuf<char,std::char_traits<char>_>::
            basic_streambuf<char,std::char_traits<char>_>
                      ((basic_streambuf<char,std::char_traits<char>_> *)&local_1630);
            local_1630 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                         vftable;
            local_15c8 = 0;
            local_15c0 = 4;
            DVar6 = GetLastError();
            pbVar12 = (basic_ostream<char,std::char_traits<char>_> *)
                      thunk_FUN_140042af0(&local_1638,"CreatePipe failed");
            std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar12,DVar6);
            thunk_FUN_14007ce40(&local_1638,local_10d8);
            uVar11 = thunk_FUN_14018d250(local_18e8);
            thunk_FUN_14018c4e0(uVar11,40000);
            thunk_FUN_14018bd10(local_18e8);
            if (0xf < local_10c0) {
              uVar21 = local_10d8[0];
              if (0xfff < local_10c0 + 1) {
                if ((local_10d8[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
                uVar21 = *(ulonglong *)(local_10d8[0] - 8);
                if (local_10d8[0] <= uVar21) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
                if (local_10d8[0] - uVar21 < 8) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
                if (0x27 < local_10d8[0] - uVar21) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
              }
              FUN_140002199(uVar21);
            }
            *(undefined ***)((longlong)&local_1638 + (longlong)*(int *)(local_1638 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_163c + (longlong)*(int *)(local_1638 + 4)) =
                 *(int *)(local_1638 + 4) + -0x88;
            local_1630 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                         vftable;
            thunk_FUN_140065bc0(&local_1630);
            std::basic_streambuf<char,std::char_traits<char>_>::
            ~basic_streambuf<char,std::char_traits<char>_>
                      ((basic_streambuf<char,std::char_traits<char>_> *)&local_1630);
            std::basic_ostream<char,std::char_traits<char>_>::
            ~basic_ostream<char,std::char_traits<char>_>(local_1628);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_15b0);
          }
          CloseHandle(local_1aa8);
        }
        else {
          BVar7 = SetHandleInformation(local_1ab0,1,0);
          if (BVar7 == 0) {
            uVar11 = thunk_FUN_14018d250(local_18d8);
            cVar4 = thunk_FUN_14018ceb0(uVar11,40000);
            thunk_FUN_14018bd10(local_18d8);
            if (cVar4 != '\0') {
              local_1458 = &DAT_14034ae60;
              std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                        (local_13d0);
              std::basic_ostream<char,std::char_traits<char>_>::
              basic_ostream<char,std::char_traits<char>_>
                        ((basic_ostream<char,std::char_traits<char>_> *)&local_1458,
                         (basic_streambuf<char,std::char_traits<char>_> *)local_1450,false);
              *(undefined ***)((longlong)local_1450 + (longlong)*(int *)(local_1458 + 4) + -8) =
                   std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                   vftable;
              *(int *)((longlong)&iStack_145c + (longlong)*(int *)(local_1458 + 4)) =
                   *(int *)(local_1458 + 4) + -0x88;
              local_1a30 = local_1450;
              std::basic_streambuf<char,std::char_traits<char>_>::
              basic_streambuf<char,std::char_traits<char>_>
                        ((basic_streambuf<char,std::char_traits<char>_> *)local_1450);
              local_1450[0] =
                   std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
              local_13e8 = 0;
              local_13e0 = 4;
              DVar6 = GetLastError();
              pbVar12 = (basic_ostream<char,std::char_traits<char>_> *)
                        thunk_FUN_140042af0(&local_1458,"SetHandleInformation failed");
              std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar12,DVar6);
              thunk_FUN_14007ce40(&local_1458,local_10b8);
              uVar11 = thunk_FUN_14018d250(local_18c8);
              thunk_FUN_14018c4e0(uVar11,40000);
              thunk_FUN_14018bd10(local_18c8);
              if (0xf < local_10a0) {
                uVar21 = local_10b8[0];
                if (0xfff < local_10a0 + 1) {
                  if ((local_10b8[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                    _invalid_parameter_noinfo_noreturn();
                  }
                  uVar21 = *(ulonglong *)(local_10b8[0] - 8);
                  if (local_10b8[0] <= uVar21) {
                    /* WARNING: Subroutine does not return */
                    _invalid_parameter_noinfo_noreturn();
                  }
                  if (local_10b8[0] - uVar21 < 8) {
                    /* WARNING: Subroutine does not return */
                    _invalid_parameter_noinfo_noreturn();
                  }
                  if (0x27 < local_10b8[0] - uVar21) {
                    /* WARNING: Subroutine does not return */
                    _invalid_parameter_noinfo_noreturn();
                  }
                }
                FUN_140002199(uVar21);
              }
              thunk_FUN_14005cb60(local_13d0);
              std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                        (local_13d0);
            }
            CloseHandle(local_1a90);
            CloseHandle(local_1ab0);
            CloseHandle(local_1aa8);
          }
          else {
            auVar3._8_8_ = 0;
            auVar3._0_8_ = (longlong)(*(int *)(local_1aa0 + 4) + 1);
            _Var14 = SUB168(ZEXT816(2) * auVar3,0);
            if (SUB168(ZEXT816(2) * auVar3,8) != 0) {
              _Var14 = 0xffffffffffffffff;
            }
            lpCommandLine = operator_new(_Var14);
            _Src = QString::utf16((QString *)&local_1aa0);
            memmove(lpCommandLine,_Src,(longlong)*(int *)(local_1aa0 + 4) * 2);
            lpCommandLine[*(int *)(local_1aa0 + 4)] = L'\0';
            local_1a20.hProcess = (HANDLE)0x0;
            local_1a20.hThread = (HANDLE)0x0;
            local_1a20.dwProcessId = 0;
            local_1a20.dwThreadId = 0;
            memset(&local_1368,0,0x68);
            local_1368.cb = 0x68;
            local_1368.dwFlags = 0x101;
            local_1368.wShowWindow = 5;
            local_1368.hStdOutput = local_1a90;
            lpCurrentDirectory = (LPCWSTR)QString::utf16(local_1a48);
            lpApplicationName = (LPCWSTR)QString::utf16(local_1a88);
            BVar7 = CreateProcessAsUserW
                              (local_1aa8,lpApplicationName,lpCommandLine,(LPSECURITY_ATTRIBUTES)0x0
                               ,(LPSECURITY_ATTRIBUTES)0x0,1,0,(LPVOID)0x0,lpCurrentDirectory,
                               &local_1368,&local_1a20);
            if (BVar7 == 0) {
              uVar11 = thunk_FUN_14018d250(local_18b8);
              cVar4 = thunk_FUN_14018ceb0(uVar11,40000);
              thunk_FUN_14018bd10(local_18b8);
              if (cVar4 != '\0') {
                local_12f8 = &DAT_14034ae60;
                std::basic_ios<char,std::char_traits<char>_>::
                basic_ios<char,std::char_traits<char>_>(local_1270);
                std::basic_ostream<char,std::char_traits<char>_>::
                basic_ostream<char,std::char_traits<char>_>
                          ((basic_ostream<char,std::char_traits<char>_> *)&local_12f8,local_12f0,
                           false);
                *(undefined ***)(local_12f0 + (longlong)*(int *)(local_12f8 + 4) + -8) =
                     std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                     vftable;
                *(int *)((longlong)&iStack_12fc + (longlong)*(int *)(local_12f8 + 4)) =
                     *(int *)(local_12f8 + 4) + -0x88;
                thunk_FUN_140058f40(local_12f0,2);
                DVar6 = GetLastError();
                pbVar12 = (basic_ostream<char,std::char_traits<char>_> *)
                          thunk_FUN_140042af0(&local_12f8,"Failed to CreateProcessAsUser: ");
                pbVar13 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                                    (pbVar12,DVar6);
                uVar11 = thunk_FUN_140042af0(pbVar13,", hNewToken: ");
                this_01 = (QByteArray *)QString::toLocal8Bit(local_1a88);
                pcVar15 = QByteArray::data(this_01);
                thunk_FUN_140042af0(uVar11,pcVar15);
                QByteArray::~QByteArray(local_19c0);
                thunk_FUN_14007ce40(&local_12f8,local_1098);
                uVar11 = thunk_FUN_14018d250(local_18a8);
                thunk_FUN_14018c4e0(uVar11,40000);
                thunk_FUN_14018bd10(local_18a8);
                if (0xf < local_1080) {
                  thunk_FUN_140065690(local_1098[0],local_1080 + 1);
                }
                thunk_FUN_14005cb60(local_1270);
                this_03 = local_1270;
LAB_140088018:
                std::basic_ios<char,std::char_traits<char>_>::
                ~basic_ios<char,std::char_traits<char>_>(this_03);
              }
            }
            else {
              WaitForSingleObject(local_1a20.hProcess,5000);
              BVar7 = ReadFile(local_1ab0,local_1038,0x1000,&local_1a7c,(LPOVERLAPPED)0x0);
              if (BVar7 == 0) {
                uVar11 = thunk_FUN_14018d250(local_1868);
                cVar4 = thunk_FUN_14018ceb0(uVar11,40000);
                thunk_FUN_14018bd10(local_1868);
                if (cVar4 != '\0') {
                  local_1208 = &DAT_14034ae60;
                  std::basic_ios<char,std::char_traits<char>_>::
                  basic_ios<char,std::char_traits<char>_>(local_1180);
                  std::basic_ostream<char,std::char_traits<char>_>::
                  basic_ostream<char,std::char_traits<char>_>
                            ((basic_ostream<char,std::char_traits<char>_> *)&local_1208,local_1200,
                             false);
                  *(undefined ***)(local_1200 + (longlong)*(int *)(local_1208 + 4) + -8) =
                       std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                       vftable;
                  *(int *)((longlong)&iStack_120c + (longlong)*(int *)(local_1208 + 4)) =
                       *(int *)(local_1208 + 4) + -0x88;
                  thunk_FUN_140058f40(local_1200,2);
                  DVar6 = GetLastError();
                  pbVar12 = (basic_ostream<char,std::char_traits<char>_> *)
                            thunk_FUN_140042af0(&local_1208,"failed to ReadFile from pipe: ");
                  std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar12,DVar6);
                  thunk_FUN_14007ce40(&local_1208,local_1058);
                  uVar11 = thunk_FUN_14018d250(local_1858);
                  thunk_FUN_14018c4e0(uVar11,40000);
                  thunk_FUN_14018bd10(local_1858);
                  thunk_FUN_14005cc00(local_1058);
                  thunk_FUN_14005cb60(local_1180);
                  this_03 = local_1180;
                  goto LAB_140088018;
                }
              }
              else {
                local_1ac8 = shared_null_exref;
                if (0xfff < (ulonglong)local_1a7c) {
                    /* WARNING: Subroutine does not return */
                  __report_rangecheckfailure();
                }
                local_1038[local_1a7c] = '\0';
                pQVar10 = (QString *)QString::number((ulong)local_19b8,uVar2);
                QVariant::QVariant(local_1998,pQVar10);
                lVar16 = -1;
                do {
                  lVar22 = lVar16 + 1;
                  pcVar15 = &DAT_14034ddcd + lVar16;
                  lVar16 = lVar22;
                } while (*pcVar15 != '\0');
                local_1a58 = QString::fromAscii_helper("uid",(int)lVar22);
                if (1 < *(uint *)local_1ac8) {
                  thunk_FUN_140088f00(&local_1ac8);
                }
                lVar16 = thunk_FUN_140089480(local_1ac8,&local_1a58);
                if (lVar16 == 0) {
                  QVariant::QVariant(local_1898);
                  plVar8 = (longlong *)thunk_FUN_140089d00(&local_1ac8,local_1838,&local_1a58);
                  lVar16 = *plVar8;
                  QVariant::~QVariant(local_1898);
                }
                QVariant::operator=((QVariant *)(lVar16 + 0x20),local_1998);
                QString::~QString((QString *)&local_1a58);
                QVariant::~QVariant(local_1998);
                QString::~QString(local_19b8);
                QByteArray::QByteArray(local_1a28,local_1038,-1);
                lVar16 = -1;
                do {
                  lVar23 = lVar16 + 1;
                  lVar22 = lVar16 + 1;
                  lVar16 = lVar23;
                } while ("proxies"[lVar22] != '\0');
                local_1a50 = QString::fromAscii_helper("proxies",(int)lVar23);
                this_02 = (QJsonDocument *)
                          QJsonDocument::fromJson
                                    ((QByteArray *)&local_1ac0,(QJsonParseError *)local_1a28);
                pQVar17 = (QVariant *)QJsonDocument::toVariant(this_02);
                if (1 < *(uint *)local_1ac8) {
                  thunk_FUN_140088f00(&local_1ac8);
                }
                lVar16 = thunk_FUN_140089480(local_1ac8,&local_1a50);
                if (lVar16 == 0) {
                  QVariant::QVariant(local_1888);
                  plVar8 = (longlong *)thunk_FUN_140089d00(&local_1ac8,local_1848);
                  lVar16 = *plVar8;
                  QVariant::~QVariant(local_1888);
                }
                QVariant::operator=((QVariant *)(lVar16 + 0x20),pQVar17);
                QVariant::~QVariant(local_1878);
                QJsonDocument::~QJsonDocument((QJsonDocument *)&local_1ac0);
                QString::~QString((QString *)&local_1a50);
                QByteArray::~QByteArray(local_1a28);
                QVariant::QVariant(local_19a8,(QMap<QString,QVariant> *)&local_1ac8);
                thunk_FUN_1400869f0(&local_1a98,local_19a8);
                QVariant::~QVariant(local_19a8);
                if (*(uint *)local_1ac8 == 0) {
LAB_140087ec1:
                  thunk_FUN_140088d00(local_1ac8);
                }
                else if (*(uint *)local_1ac8 != 0xffffffff) {
                  LOCK();
                  uVar26 = *(uint *)local_1ac8;
                  *(uint *)local_1ac8 = *(uint *)local_1ac8 - 1;
                  UNLOCK();
                  pNVar25 = local_1a78;
                  pNVar18 = local_1a70;
                  if (uVar26 == 1) goto LAB_140087ec1;
                }
              }
            }
            CloseHandle(local_1a90);
            CloseHandle(local_1ab0);
            CloseHandle(local_1a20.hProcess);
            CloseHandle(local_1a20.hThread);
            CloseHandle(local_1aa8);
            thunk_FUN_140002199(lpCommandLine);
          }
        }
      }
      local_1a60 = 0;
    }
    uVar26 = local_1a60;
    pNVar18 = QHashData::nextNode(pNVar18);
    local_1a60 = uVar26 ^ 1;
    local_1a70 = pNVar18;
  } while (local_1a60 != 0);
  if (*(int *)(pNVar25 + 0x10) == 0) {
LAB_1400880ff:
    QHashData::free_helper((QHashData *)pNVar25,thunk_FUN_140088b70);
  }
  else if (*(int *)(pNVar25 + 0x10) != -1) {
    LOCK();
    pNVar25 = pNVar25 + 0x10;
    iVar5 = *(int *)pNVar25;
    *(int *)pNVar25 = *(int *)pNVar25 + -1;
    UNLOCK();
    pNVar25 = local_1a78;
    if (iVar5 == 1) goto LAB_1400880ff;
  }
  uVar11 = local_19b0;
  FUN_14008a2d0(local_19b0,&local_1a98);
  QString::~QString((QString *)&local_1aa0);
  QString::~QString(local_1a48);
  if (*(int *)local_1a98 != 0) {
    if (*(int *)local_1a98 == -1) goto LAB_140088168;
    LOCK();
    iVar5 = *(int *)local_1a98;
    *(int *)local_1a98 = *(int *)local_1a98 + -1;
    UNLOCK();
    if (iVar5 != 1) goto LAB_140088168;
  }
  thunk_FUN_140088ac0(&local_1a98,local_1a98);
LAB_140088168:
  QString::~QString(local_1a88);
  QString::~QString(local_1a40);
  if (*(int *)(local_1ad0 + 0x10) != 0) {
    if (*(int *)(local_1ad0 + 0x10) == -1) {
      return uVar11;
    }
    LOCK();
    pQVar1 = (QHashData *)(local_1ad0 + 0x10);
    iVar5 = *(int *)pQVar1;
    *(int *)pQVar1 = *(int *)pQVar1 + -1;
    UNLOCK();
    if (iVar5 != 1) {
      return uVar11;
    }
  }
  QHashData::free_helper((QHashData *)local_1ad0,thunk_FUN_140088b70);
  return uVar11;
}

// =============================================================================
// Ghidra: FUN_14008e8c0 @ 0x14008e8c0
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14008e8c0(undefined8 param_1,int param_2)

{
  DWORD DVar1;
  BOOL BVar2;
  SC_HANDLE hSCManager;
  basic_ostream<char,std::char_traits<char>_> *pbVar3;
  basic_ostream<char,struct_std::char_traits<char>_> *pbVar4;
  LPCWSTR lpServiceName;
  SC_HANDLE hService;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined1 auStackY_c8 [32];
  QString local_68 [8];
  undefined8 local_60;
  undefined2 local_58 [8];
  undefined8 local_48;
  ulonglong local_40;
  undefined1 local_38 [32];
  ulonglong local_18;
  
  local_60 = 0xfffffffffffffffe;
  local_18 = DAT_14050e9d0 ^ (ulonglong)auStackY_c8;
  FUN_14008ed80();
  if (param_2 == 1) {
    hSCManager = OpenSCManagerW((LPCWSTR)0x0,(LPCWSTR)0x0,4);
    if (hSCManager == (SC_HANDLE)0x0) {
      DVar1 = GetLastError();
      pbVar3 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_140042af0(cerr_exref,"OpenSCManagerW failed: ");
      pbVar4 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar3,DVar1);
      std::basic_ostream<char,std::char_traits<char>_>::operator<<
                ((basic_ostream<char,std::char_traits<char>_> *)pbVar4,thunk_FUN_140083cd0);
    }
    else {
      lpServiceName = (LPCWSTR)QString::toStdWString(local_68);
      if (7 < *(ulonglong *)(lpServiceName + 0xc)) {
        lpServiceName = *(LPCWSTR *)lpServiceName;
      }
      hService = OpenServiceW(hSCManager,lpServiceName,3);
      if (7 < local_40) {
        thunk_FUN_14008e2d0();
      }
      local_40 = 7;
      local_48 = 0;
      local_58[0] = 0;
      if (hService == (SC_HANDLE)0x0) {
        uVar5 = QString::toStdString(local_68);
        DVar1 = GetLastError();
        uVar6 = thunk_FUN_140042af0(cerr_exref,"OpenServiceW (");
        uVar5 = thunk_FUN_14008b520(uVar6,uVar5);
        pbVar3 = (basic_ostream<char,std::char_traits<char>_> *)
                 thunk_FUN_140042af0(uVar5,") failed: ");
        pbVar4 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar3,DVar1);
        std::basic_ostream<char,std::char_traits<char>_>::operator<<
                  ((basic_ostream<char,std::char_traits<char>_> *)pbVar4,thunk_FUN_140083cd0);
        thunk_FUN_14005cc00(local_58);
      }
      else {
        BVar2 = ChangeServiceConfigW
                          (hService,0xffffffff,0xffffffff,0xffffffff,(LPCWSTR)0x0,(LPCWSTR)0x0,
                           (LPDWORD)0x0,L"TermService",(LPCWSTR)0x0,(LPCWSTR)0x0,(LPCWSTR)0x0);
        if (BVar2 == 0) {
          uVar5 = QString::toStdString(local_68);
          DVar1 = GetLastError();
          uVar6 = thunk_FUN_140042af0(cerr_exref,"ChangeServiceConfigW (");
          uVar5 = thunk_FUN_14008b520(uVar6,uVar5);
          pbVar3 = (basic_ostream<char,std::char_traits<char>_> *)
                   thunk_FUN_140042af0(uVar5,") failed: ");
          pbVar4 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar3,DVar1);
          std::basic_ostream<char,std::char_traits<char>_>::operator<<
                    ((basic_ostream<char,std::char_traits<char>_> *)pbVar4,thunk_FUN_140083cd0);
          thunk_FUN_14005cc00(local_38);
        }
        CloseServiceHandle(hService);
      }
      CloseServiceHandle(hSCManager);
    }
  }
  QString::~QString(local_68);
  return;
}

// =============================================================================
// Ghidra: FUN_1400914e0 @ 0x1400914e0
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1400914e0(QObject *param_1)

{
  char cVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  longlong *plVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  undefined ***pppuVar8;
  char *pcVar9;
  char *pcVar10;
  QObject *pQVar11;
  uint uVar12;
  undefined1 auStackY_1e8 [32];
  undefined ***local_1b8;
  undefined4 uStack_1b0;
  int iStack_1ac;
  uint local_1a8 [4];
  undefined4 local_198;
  undefined4 uStack_194;
  undefined4 uStack_190;
  int iStack_18c;
  undefined *local_188;
  undefined **local_180;
  basic_ostream<char,std::char_traits<char>_> local_178 [96];
  undefined8 local_118;
  undefined4 local_110;
  basic_ios<char,std::char_traits<char>_> local_100 [104];
  QMutexLocker local_98 [8];
  QString local_90 [8];
  QString local_88 [8];
  QString local_80 [8];
  QString local_78 [8];
  QString local_70 [8];
  undefined8 local_68;
  QObject *local_58;
  undefined4 uStack_50;
  int iStack_4c;
  ulonglong local_40;
  ulonglong local_38;
  
  local_68 = 0xfffffffffffffffe;
  local_38 = DAT_14050e9d0 ^ (ulonglong)auStackY_1e8;
  uVar7 = 0;
  uVar12 = 0;
  local_1a8[0] = 0;
  QMutexLocker::QMutexLocker(local_98,(QBasicMutex *)(param_1 + 0x50));
  uVar2 = thunk_FUN_14018d250(&local_1b8);
  cVar1 = thunk_FUN_14018ceb0(uVar2,20000);
  thunk_FUN_14018bd10(&local_1b8);
  if (cVar1 != '\0') {
    local_188 = &DAT_14034ae60;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_100)
    ;
    uVar12 = 1;
    local_1a8[0] = 1;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_188,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_180,false);
    *(undefined ***)((longlong)&local_188 + (longlong)*(int *)(local_188 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_18c + (longlong)*(int *)(local_188 + 4)) =
         *(int *)(local_188 + 4) + -0x88;
    local_1b8 = &local_180;
    std::basic_streambuf<char,std::char_traits<char>_>::
    basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_180);
    local_180 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    local_118 = 0;
    local_110 = 4;
    thunk_FUN_140042af0(&local_188,"start");
    uVar2 = thunk_FUN_14007ce40(&local_188,&local_58);
    uVar3 = thunk_FUN_14018d250(&local_1b8);
    thunk_FUN_14018c4e0(uVar3,20000,uVar2,
                        "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\app-grabber\\service.cpp");
    thunk_FUN_14018bd10(&local_1b8);
    if (0xf < local_40) {
      pQVar11 = local_58;
      if (0xfff < local_40 + 1) {
        if (((ulonglong)local_58 & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        pQVar11 = *(QObject **)(local_58 + -8);
        if (local_58 <= pQVar11) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if ((ulonglong)((longlong)local_58 - (longlong)pQVar11) < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < (ulonglong)((longlong)local_58 - (longlong)pQVar11)) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002199(pQVar11);
    }
    *(undefined ***)((longlong)&local_188 + (longlong)*(int *)(local_188 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_18c + (longlong)*(int *)(local_188 + 4)) =
         *(int *)(local_188 + 4) + -0x88;
    local_180 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_140065bc0(&local_180);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_180);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_178);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
              (local_100);
  }
  plVar4 = (longlong *)thunk_FUN_140076760();
  (**(code **)(*plVar4 + 0x58))(plVar4);
  local_1b8 = (undefined ***)thunk_FUN_14008fae0;
  uStack_1b0 = 0;
  local_58 = (QObject *)thunk_FUN_1400ae3e0;
  uStack_50 = 0;
  uVar2 = thunk_FUN_140076760();
  thunk_FUN_14008ba90(local_1a8,uVar2,&local_58,param_1);
  QMetaObject::Connection::~Connection((Connection *)local_1a8);
  local_1b8 = (undefined ***)FUN_140005308;
  uStack_1b0 = 0;
  local_58 = (QObject *)thunk_FUN_1400aed10;
  uStack_50 = 0;
  uVar2 = thunk_FUN_140076760();
  thunk_FUN_14008b8b0(local_1a8,uVar2,&local_58,param_1);
  QMetaObject::Connection::~Connection((Connection *)local_1a8);
  local_1b8 = operator_new(0x70);
  uVar2 = uVar7;
  if (local_1b8 != (undefined ***)0x0) {
    uVar2 = thunk_FUN_1400e1760(local_1b8);
  }
  *(undefined8 *)(param_1 + 0x70) = uVar2;
  uVar2 = thunk_FUN_140086b60(&local_1b8);
  uVar3 = thunk_FUN_140076760();
  uVar3 = thunk_FUN_1400a4e00(uVar3,&local_198);
  uVar5 = thunk_FUN_140076760();
  uVar5 = thunk_FUN_1400a4d90(uVar5,local_1a8);
  uVar6 = thunk_FUN_140076800();
  thunk_FUN_14012de60(uVar6,uVar5,uVar3,uVar2);
  QString::~QString((QString *)local_1a8);
  QString::~QString((QString *)&local_198);
  thunk_FUN_140088bb0(CONCAT44(iStack_1ac,uStack_1b0));
  local_1b8 = operator_new(0x38);
  uVar2 = uVar7;
  if (local_1b8 != (undefined ***)0x0) {
    uVar2 = thunk_FUN_140153f70(local_1b8);
  }
  *(undefined8 *)(param_1 + 0x60) = uVar2;
  uVar2 = thunk_FUN_14008e870(&local_198);
  uVar2 = thunk_FUN_140089540(local_90,uVar2);
  uVar3 = thunk_FUN_140084f60(&local_1b8,9);
  uVar3 = thunk_FUN_140060f80(&local_58,uVar3,&DAT_14034d404);
  uVar2 = thunk_FUN_140060f10(local_1a8,uVar3,uVar2);
  thunk_FUN_140076760();
  cVar1 = thunk_FUN_140156a90(*(undefined8 *)(param_1 + 0x60),uVar2);
  QString::~QString((QString *)local_1a8);
  QString::~QString((QString *)&local_58);
  QString::~QString((QString *)&local_1b8);
  QString::~QString(local_70);
  QString::~QString(local_78);
  QString::~QString(local_80);
  QString::~QString(local_88);
  QString::~QString(local_90);
  if (cVar1 != '\0') {
    local_1b8 = operator_new(0x30);
    uVar2 = uVar7;
    if (local_1b8 != (undefined ***)0x0) {
      uVar2 = thunk_FUN_1401574c0(local_1b8,*(undefined8 *)(param_1 + 0x60));
    }
    *(undefined8 *)(param_1 + 0x58) = uVar2;
    local_1b8 = (undefined ***)thunk_FUN_14008f0d0;
    uStack_1b0 = 0;
    local_58 = (QObject *)thunk_FUN_14008f0d0;
    uStack_50 = 0;
    iStack_4c = iStack_1ac;
    thunk_FUN_14008b7c0(&local_1b8,uVar2,thunk_FUN_14015cd10,param_1);
    QMetaObject::Connection::~Connection((Connection *)&local_1b8);
    thunk_FUN_140076760();
    thunk_FUN_140076760();
    uVar2 = QCoreApplication::applicationFilePath();
    uVar3 = thunk_FUN_14009c770(&local_198);
    uVar5 = thunk_FUN_14009c7f0(&local_58);
    uVar3 = thunk_FUN_140060f10(&local_1b8,uVar5,uVar3);
    thunk_FUN_140157b00(*(undefined8 *)(param_1 + 0x58),uVar3,uVar2);
    QString::~QString((QString *)&local_1b8);
    QString::~QString((QString *)&local_58);
    QString::~QString((QString *)&local_198);
    QString::~QString((QString *)local_1a8);
    thunk_FUN_140159e60(*(undefined8 *)(param_1 + 0x58));
    local_1b8 = (undefined ***)thunk_FUN_1400ae2c0;
    uStack_1b0 = 0;
    local_198 = 0x40013aed;
    uStack_194 = 1;
    uStack_190 = 0;
    iStack_18c = iStack_1ac;
    local_58 = param_1;
    uVar2 = thunk_FUN_140076760();
    thunk_FUN_14008bb80(&local_1b8,uVar2,&local_198,*(undefined8 *)(param_1 + 0x60));
    QMetaObject::Connection::~Connection((Connection *)&local_1b8);
    FUN_14008cb40(&local_58);
    pppuVar8 = operator_new(0x1a0);
    uVar2 = uVar7;
    local_1b8 = pppuVar8;
    if (pppuVar8 != (undefined ***)0x0) {
      uVar2 = thunk_FUN_140076760();
      uVar2 = thunk_FUN_14017b6b0(pppuVar8,uVar2);
    }
    *(undefined8 *)(param_1 + 0x68) = uVar2;
    local_1b8 = operator_new(0x10);
    if (local_1b8 != (undefined ***)0x0) {
      uVar7 = thunk_FUN_140092e80(local_1b8);
    }
    *(undefined8 *)(param_1 + 0x80) = uVar7;
    pcVar9 = qFlagLocation("1onSuspend()");
    pcVar10 = qFlagLocation("2suspend()");
    QObject::connect(param_1,(QObject *)&local_1b8,*(char **)(param_1 + 0x80),pcVar10,
                     (ConnectionType)pcVar9);
    QMetaObject::Connection::~Connection((Connection *)&local_1b8);
    pcVar9 = qFlagLocation("1onResumeSuspend()");
    pcVar10 = qFlagLocation("2resumeSuspend()");
    QObject::connect(param_1,(QObject *)&local_1b8,*(char **)(param_1 + 0x80),pcVar10,
                     (ConnectionType)pcVar9);
    QMetaObject::Connection::~Connection((Connection *)&local_1b8);
    uVar2 = thunk_FUN_140090080();
    uVar7 = thunk_FUN_140076760();
    thunk_FUN_1400d9c30(uVar2,uVar7);
    uVar2 = thunk_FUN_14018d250(&local_1b8);
    cVar1 = thunk_FUN_14018ceb0(uVar2,20000);
    thunk_FUN_14018bd10(&local_1b8);
    if (cVar1 != '\0') {
      local_188 = &DAT_14034ae60;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_100);
      local_1a8[0] = uVar12 | 4;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_188,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_180,false);
      *(undefined ***)((longlong)&local_188 + (longlong)*(int *)(local_188 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_18c + (longlong)*(int *)(local_188 + 4)) =
           *(int *)(local_188 + 4) + -0x88;
      local_1b8 = &local_180;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_180);
      local_180 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_118 = 0;
      local_110 = 4;
      thunk_FUN_140042af0(&local_188,"started");
      uVar2 = thunk_FUN_14007ce40(&local_188,&local_58);
      uVar7 = thunk_FUN_14018d250(&local_1b8);
      thunk_FUN_14018c4e0(uVar7,20000,uVar2,
                          "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\app-grabber\\service.cpp");
      thunk_FUN_14018bd10(&local_1b8);
      if (0xf < local_40) {
        pQVar11 = local_58;
        if (0xfff < local_40 + 1) {
          if (((ulonglong)local_58 & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          pQVar11 = *(QObject **)(local_58 + -8);
          if (local_58 <= pQVar11) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if ((ulonglong)((longlong)local_58 - (longlong)pQVar11) < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < (ulonglong)((longlong)local_58 - (longlong)pQVar11)) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002199(pQVar11);
      }
      *(undefined ***)((longlong)&local_188 + (longlong)*(int *)(local_188 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_18c + (longlong)*(int *)(local_188 + 4)) =
           *(int *)(local_188 + 4) + -0x88;
      local_180 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140065bc0(&local_180);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_180);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_178);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_100);
    }
    QMutexLocker::~QMutexLocker(local_98);
    return;
  }
  uVar2 = thunk_FUN_14018d250(&local_1b8);
  cVar1 = thunk_FUN_14018ceb0(uVar2,50000);
  thunk_FUN_14018bd10(&local_1b8);
  if (cVar1 != '\0') {
    local_188 = &DAT_14034ae60;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_100)
    ;
    local_1a8[0] = uVar12 | 2;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_188,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_180,false);
    *(undefined ***)((longlong)&local_188 + (longlong)*(int *)(local_188 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_18c + (longlong)*(int *)(local_188 + 4)) =
         *(int *)(local_188 + 4) + -0x88;
    local_1b8 = &local_180;
    std::basic_streambuf<char,std::char_traits<char>_>::
    basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_180);
    local_180 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    local_118 = 0;
    local_110 = 4;
    thunk_FUN_140042af0(&local_188,"unable to setup aup collector");
    uVar2 = thunk_FUN_14007ce40(&local_188,&local_58);
    uVar7 = thunk_FUN_14018d250(&local_1b8);
    thunk_FUN_14018c4e0(uVar7,50000,uVar2,
                        "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\app-grabber\\service.cpp");
    thunk_FUN_14018bd10(&local_1b8);
    if (0xf < local_40) {
      pQVar11 = local_58;
      if (0xfff < local_40 + 1) {
        if (((ulonglong)local_58 & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        pQVar11 = *(QObject **)(local_58 + -8);
        if (local_58 <= pQVar11) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if ((ulonglong)((longlong)local_58 - (longlong)pQVar11) < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < (ulonglong)((longlong)local_58 - (longlong)pQVar11)) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002199(pQVar11);
    }
    *(undefined ***)((longlong)&local_188 + (longlong)*(int *)(local_188 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_18c + (longlong)*(int *)(local_188 + 4)) =
         *(int *)(local_188 + 4) + -0x88;
    local_180 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_140065bc0(&local_180);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_180);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_178);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
              (local_100);
  }
                    /* WARNING: Subroutine does not return */
  exit(-0x23);
}

// =============================================================================
// Ghidra: FUN_140092270 @ 0x140092270
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140092270(longlong param_1)

{
  char cVar1;
  bool bVar2;
  bool bVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  longlong *plVar6;
  ulonglong uVar7;
  uint uVar8;
  undefined1 auStack_198 [32];
  undefined4 local_178;
  uint local_168;
  undefined ***local_160 [2];
  int iStack_14c;
  undefined *local_148;
  undefined **local_140;
  basic_ostream<char,std::char_traits<char>_> local_138 [96];
  undefined8 local_d8;
  undefined4 local_d0;
  basic_ios<char,std::char_traits<char>_> local_c0 [104];
  undefined8 local_58;
  ulonglong local_50 [3];
  ulonglong local_38;
  ulonglong local_30;
  
  local_58 = 0xfffffffffffffffe;
  local_30 = DAT_14050e9d0 ^ (ulonglong)auStack_198;
  uVar8 = 0;
  local_168 = 0;
  uVar4 = thunk_FUN_14018d250(local_160);
  cVar1 = thunk_FUN_14018ceb0(uVar4);
  thunk_FUN_14018bd10(local_160);
  if (cVar1 != '\0') {
    local_148 = &DAT_14034ae60;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_c0);
    uVar8 = 1;
    local_168 = 1;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_148,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_140,false);
    *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
         *(int *)(local_148 + 4) + -0x88;
    local_160[0] = &local_140;
    std::basic_streambuf<char,std::char_traits<char>_>::
    basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
    local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    local_d8 = 0;
    local_d0 = 4;
    thunk_FUN_140042af0(&local_148,&DAT_14034eb84);
    uVar4 = thunk_FUN_14007ce40(&local_148,local_50);
    uVar5 = thunk_FUN_14018d250(local_160);
    local_178 = 0xfa;
    thunk_FUN_14018c4e0(uVar5,20000,uVar4,
                        "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\app-grabber\\service.cpp");
    thunk_FUN_14018bd10(local_160);
    if (0xf < local_38) {
      uVar7 = local_50[0];
      if (0xfff < local_38 + 1) {
        if ((local_50[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar7 = *(ulonglong *)(local_50[0] - 8);
        if (local_50[0] <= uVar7) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_50[0] - uVar7 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_50[0] - uVar7) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002199(uVar7);
    }
    *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
         *(int *)(local_148 + 4) + -0x88;
    local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_140065bc0(&local_140);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_138);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_c0)
    ;
  }
  bVar3 = false;
  bVar2 = QMutex::tryLock((QMutex *)(param_1 + 0x40),0);
  if (!bVar2) {
    uVar4 = thunk_FUN_14018d250(local_160);
    cVar1 = thunk_FUN_14018ceb0(uVar4);
    thunk_FUN_14018bd10(local_160);
    if (cVar1 != '\0') {
      local_148 = &DAT_14034ae60;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_c0);
      uVar8 = uVar8 | 2;
      local_168 = uVar8;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_148,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_140,false);
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      local_160[0] = &local_140;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_d8 = 0;
      local_d0 = 4;
      thunk_FUN_140042af0(&local_148,"await startInitialization to be completed");
      uVar4 = thunk_FUN_14007ce40(&local_148,local_50);
      uVar5 = thunk_FUN_14018d250(local_160);
      local_178 = 0x100;
      thunk_FUN_14018c4e0(uVar5,20000,uVar4,
                          "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\app-grabber\\service.cpp");
      thunk_FUN_14018bd10(local_160);
      if (0xf < local_38) {
        uVar7 = local_50[0];
        if (0xfff < local_38 + 1) {
          if ((local_50[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar7 = *(ulonglong *)(local_50[0] - 8);
          if (local_50[0] <= uVar7) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_50[0] - uVar7 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_50[0] - uVar7) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002199(uVar7);
      }
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140065bc0(&local_140);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_138);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_c0);
    }
    bVar3 = QMutex::tryLock((QMutex *)(param_1 + 0x40),0);
    while (!bVar3) {
      QCoreApplication::processEvents(0);
      QThread::msleep(10);
      bVar3 = QMutex::tryLock((QMutex *)(param_1 + 0x40),0);
    }
    bVar3 = true;
  }
  QMutex::unlock((QMutex *)(param_1 + 0x40));
  if (bVar3) {
    uVar4 = thunk_FUN_14018d250(local_160);
    cVar1 = thunk_FUN_14018ceb0(uVar4,20000);
    thunk_FUN_14018bd10(local_160);
    if (cVar1 != '\0') {
      local_148 = &DAT_14034ae60;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_c0);
      uVar8 = uVar8 | 4;
      local_168 = uVar8;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_148,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_140,false);
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      local_160[0] = &local_140;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_d8 = 0;
      local_d0 = 4;
      thunk_FUN_140042af0(&local_148,"startInitialization completed, do real stop now");
      uVar4 = thunk_FUN_14007ce40(&local_148,local_50);
      uVar5 = thunk_FUN_14018d250(local_160);
      local_178 = 0x10e;
      thunk_FUN_14018c4e0(uVar5,20000,uVar4,
                          "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\app-grabber\\service.cpp");
      thunk_FUN_14018bd10(local_160);
      if (0xf < local_38) {
        uVar7 = local_50[0];
        if (0xfff < local_38 + 1) {
          if ((local_50[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar7 = *(ulonglong *)(local_50[0] - 8);
          if (local_50[0] <= uVar7) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_50[0] - uVar7 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_50[0] - uVar7) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002199(uVar7);
      }
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140065bc0(&local_140);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_138);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_c0);
    }
  }
  uVar4 = thunk_FUN_140090080();
  thunk_FUN_1400d9d90(uVar4);
  if (*(longlong *)(param_1 + 0x60) != 0) {
    thunk_FUN_1400e75f0();
    plVar6 = *(longlong **)(param_1 + 0x60);
    if (plVar6 != (longlong *)0x0) {
      (**(code **)(*plVar6 + 0x18))(plVar6,1);
    }
    *(undefined8 *)(param_1 + 0x60) = 0;
  }
  plVar6 = *(longlong **)(param_1 + 0x48);
  if (plVar6 != (longlong *)0x0) {
    (**(code **)(*plVar6 + 0x18))(plVar6,1);
    *(undefined8 *)(param_1 + 0x48) = 0;
  }
  plVar6 = *(longlong **)(param_1 + 0x50);
  if (plVar6 != (longlong *)0x0) {
    (**(code **)(*plVar6 + 0x18))(plVar6,1);
    *(undefined8 *)(param_1 + 0x50) = 0;
  }
  plVar6 = *(longlong **)(param_1 + 0x58);
  if (plVar6 != (longlong *)0x0) {
    (**(code **)(*plVar6 + 0x18))(plVar6,1);
    *(undefined8 *)(param_1 + 0x58) = 0;
  }
  plVar6 = (longlong *)thunk_FUN_140076760();
  (**(code **)(*plVar6 + 0x60))(plVar6);
  plVar6 = *(longlong **)(param_1 + 0x70);
  if (plVar6 != (longlong *)0x0) {
    (**(code **)(*plVar6 + 0x18))(plVar6,1);
    *(undefined8 *)(param_1 + 0x70) = 0;
  }
  uVar4 = thunk_FUN_14018d250(local_160);
  cVar1 = thunk_FUN_14018ceb0(uVar4,20000);
  thunk_FUN_14018bd10(local_160);
  if (cVar1 != '\0') {
    local_148 = &DAT_14034ae60;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_c0);
    local_168 = uVar8 | 8;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_148,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_140,false);
    *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
         *(int *)(local_148 + 4) + -0x88;
    local_160[0] = &local_140;
    std::basic_streambuf<char,std::char_traits<char>_>::
    basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
    local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    local_d8 = 0;
    local_d0 = 4;
    thunk_FUN_140042af0(&local_148,"stopped");
    uVar4 = thunk_FUN_14007ce40(&local_148,local_50);
    uVar5 = thunk_FUN_14018d250(local_160);
    local_178 = 0x137;
    thunk_FUN_14018c4e0(uVar5,20000,uVar4,
                        "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\app-grabber\\service.cpp");
    thunk_FUN_14018bd10(local_160);
    if (0xf < local_38) {
      uVar7 = local_50[0];
      if (0xfff < local_38 + 1) {
        if ((local_50[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar7 = *(ulonglong *)(local_50[0] - 8);
        if (local_50[0] <= uVar7) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_50[0] - uVar7 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_50[0] - uVar7) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002199(uVar7);
    }
    *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
         *(int *)(local_148 + 4) + -0x88;
    local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_140065bc0(&local_140);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_138);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_c0)
    ;
  }
  return;
}

// =============================================================================
// Ghidra: FUN_14008f800 @ 0x14008f800
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14008f800(longlong param_1)

{
  char cVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  longlong *plVar4;
  ulonglong uVar5;
  undefined1 auStack_188 [32];
  undefined4 local_168;
  undefined4 local_158;
  undefined ***local_150 [2];
  undefined8 local_140;
  undefined *local_138;
  undefined **local_130;
  basic_ostream<char,std::char_traits<char>_> local_128 [96];
  undefined8 local_c8;
  undefined4 local_c0;
  basic_ios<char,std::char_traits<char>_> local_b0 [104];
  ulonglong local_48 [3];
  ulonglong local_30;
  ulonglong local_28;
  
  local_140 = 0xfffffffffffffffe;
  local_28 = DAT_14050e9d0 ^ (ulonglong)auStack_188;
  local_158 = 0;
  if (*(char *)(param_1 + 0x78) == '\0') {
    *(undefined1 *)(param_1 + 0x78) = 1;
    uVar2 = thunk_FUN_14018d250(local_150);
    cVar1 = thunk_FUN_14018ceb0(uVar2,20000);
    thunk_FUN_14018bd10(local_150);
    if (cVar1 != '\0') {
      local_138 = &DAT_14034ae60;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_b0);
      local_158 = 1;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_138,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_130,false);
      *(undefined ***)((longlong)&local_138 + (longlong)*(int *)(local_138 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_140 + (longlong)*(int *)(local_138 + 4) + 4) =
           *(int *)(local_138 + 4) + -0x88;
      local_150[0] = &local_130;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_130);
      local_130 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_c8 = 0;
      local_c0 = 4;
      thunk_FUN_140042af0(&local_138,"suspend");
      uVar2 = thunk_FUN_14007ce40(&local_138,local_48);
      uVar3 = thunk_FUN_14018d250(local_150);
      local_168 = 0x143;
      thunk_FUN_14018c4e0(uVar3,20000,uVar2,
                          "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\app-grabber\\service.cpp");
      thunk_FUN_14018bd10(local_150);
      if (0xf < local_30) {
        uVar5 = local_48[0];
        if (0xfff < local_30 + 1) {
          if ((local_48[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar5 = *(ulonglong *)(local_48[0] - 8);
          if (local_48[0] <= uVar5) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_48[0] - uVar5 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_48[0] - uVar5) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002199(uVar5);
      }
      *(undefined ***)((longlong)&local_138 + (longlong)*(int *)(local_138 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_140 + (longlong)*(int *)(local_138 + 4) + 4) =
           *(int *)(local_138 + 4) + -0x88;
      local_130 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140065bc0(&local_130);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_130);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_128);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_b0);
    }
    plVar4 = (longlong *)thunk_FUN_140076760();
    (**(code **)(*plVar4 + 0x68))(plVar4);
    if (*(longlong *)(param_1 + 0x70) != 0) {
      thunk_FUN_1400e7690();
    }
  }
  return;
}

// =============================================================================
// Ghidra: FUN_14008f520 @ 0x14008f520
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14008f520(longlong param_1)

{
  char cVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  longlong *plVar4;
  ulonglong uVar5;
  undefined1 auStack_188 [32];
  undefined4 local_168;
  undefined4 local_158;
  undefined ***local_150 [2];
  undefined8 local_140;
  undefined *local_138;
  undefined **local_130;
  basic_ostream<char,std::char_traits<char>_> local_128 [96];
  undefined8 local_c8;
  undefined4 local_c0;
  basic_ios<char,std::char_traits<char>_> local_b0 [104];
  ulonglong local_48 [3];
  ulonglong local_30;
  ulonglong local_28;
  
  local_140 = 0xfffffffffffffffe;
  local_28 = DAT_14050e9d0 ^ (ulonglong)auStack_188;
  local_158 = 0;
  if (*(char *)(param_1 + 0x78) != '\0') {
    *(undefined1 *)(param_1 + 0x78) = 0;
    uVar2 = thunk_FUN_14018d250(local_150);
    cVar1 = thunk_FUN_14018ceb0(uVar2,20000);
    thunk_FUN_14018bd10(local_150);
    if (cVar1 != '\0') {
      local_138 = &DAT_14034ae60;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_b0);
      local_158 = 1;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_138,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_130,false);
      *(undefined ***)((longlong)&local_138 + (longlong)*(int *)(local_138 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_140 + (longlong)*(int *)(local_138 + 4) + 4) =
           *(int *)(local_138 + 4) + -0x88;
      local_150[0] = &local_130;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_130);
      local_130 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_c8 = 0;
      local_c0 = 4;
      thunk_FUN_140042af0(&local_138,"resume suspend");
      uVar2 = thunk_FUN_14007ce40(&local_138,local_48);
      uVar3 = thunk_FUN_14018d250(local_150);
      local_168 = 0x156;
      thunk_FUN_14018c4e0(uVar3,20000,uVar2,
                          "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\app-grabber\\service.cpp");
      thunk_FUN_14018bd10(local_150);
      if (0xf < local_30) {
        uVar5 = local_48[0];
        if (0xfff < local_30 + 1) {
          if ((local_48[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar5 = *(ulonglong *)(local_48[0] - 8);
          if (local_48[0] <= uVar5) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_48[0] - uVar5 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_48[0] - uVar5) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002199(uVar5);
      }
      *(undefined ***)((longlong)&local_138 + (longlong)*(int *)(local_138 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_140 + (longlong)*(int *)(local_138 + 4) + 4) =
           *(int *)(local_138 + 4) + -0x88;
      local_130 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140065bc0(&local_130);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_130);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_128);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_b0);
    }
    plVar4 = (longlong *)thunk_FUN_140076760();
    (**(code **)(*plVar4 + 0x70))(plVar4);
    if (*(longlong *)(param_1 + 0x70) != 0) {
      thunk_FUN_1400e6e30();
    }
  }
  return;
}

// =============================================================================
// Ghidra: FUN_14008f0d0 @ 0x14008f0d0
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14008f0d0(longlong param_1)

{
  QString *pQVar1;
  int iVar2;
  Data *pDVar3;
  char cVar4;
  undefined8 uVar5;
  QString *pQVar6;
  QByteArray *this;
  char *pcVar7;
  undefined8 uVar8;
  longlong *plVar9;
  ulonglong uVar10;
  undefined1 auStack_1c8 [32];
  undefined4 local_1a8;
  Data *local_198;
  undefined4 local_190;
  QTypedArrayData<unsigned_short> *local_188;
  undefined ***local_180;
  QByteArray local_178 [8];
  QString local_170 [8];
  undefined8 local_168;
  undefined1 local_160 [16];
  undefined1 local_150 [20];
  int iStack_13c;
  undefined *local_138;
  undefined **local_130;
  basic_ostream<char,std::char_traits<char>_> local_128 [96];
  undefined8 local_c8;
  undefined4 local_c0;
  basic_ios<char,std::char_traits<char>_> local_b0 [104];
  ulonglong local_48 [3];
  ulonglong local_30;
  ulonglong local_28;
  
  local_168 = 0xfffffffffffffffe;
  local_28 = DAT_14050e9d0 ^ (ulonglong)auStack_1c8;
  local_190 = 0;
  uVar5 = thunk_FUN_14018d250(local_160);
  cVar4 = thunk_FUN_14018ceb0(uVar5,20000);
  thunk_FUN_14018bd10(local_160);
  if (cVar4 != '\0') {
    local_138 = &DAT_14034ae60;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_b0);
    local_190 = 1;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_138,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_130,false);
    *(undefined ***)((longlong)&local_138 + (longlong)*(int *)(local_138 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_13c + (longlong)*(int *)(local_138 + 4)) =
         *(int *)(local_138 + 4) + -0x88;
    local_180 = &local_130;
    std::basic_streambuf<char,std::char_traits<char>_>::
    basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_130);
    local_130 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    local_c8 = 0;
    local_c0 = 4;
    pQVar6 = (QString *)QCoreApplication::applicationFilePath();
    uVar5 = thunk_FUN_140042af0(&local_138,"restart now (after update): ");
    this = (QByteArray *)QString::toLocal8Bit(pQVar6);
    pcVar7 = QByteArray::data(this);
    thunk_FUN_140042af0(uVar5,pcVar7);
    QByteArray::~QByteArray(local_178);
    QString::~QString(local_170);
    uVar5 = thunk_FUN_14007ce40(&local_138,local_48);
    uVar8 = thunk_FUN_14018d250(local_150);
    local_1a8 = 0x167;
    thunk_FUN_14018c4e0(uVar8,20000,uVar5,
                        "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\app-grabber\\service.cpp");
    thunk_FUN_14018bd10(local_150);
    if (0xf < local_30) {
      uVar10 = local_48[0];
      if (0xfff < local_30 + 1) {
        if ((local_48[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar10 = *(ulonglong *)(local_48[0] - 8);
        if (local_48[0] <= uVar10) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_48[0] - uVar10 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_48[0] - uVar10) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002199(uVar10);
    }
    *(undefined ***)((longlong)&local_138 + (longlong)*(int *)(local_138 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_13c + (longlong)*(int *)(local_138 + 4)) =
         *(int *)(local_138 + 4) + -0x88;
    local_130 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_140065bc0(&local_130);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_130);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_128);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_b0)
    ;
  }
  if (DAT_14051ef80 != 0) {
    plVar9 = (longlong *)thunk_FUN_140076760();
    (**(code **)(*plVar9 + 0x60))(plVar9);
  }
  if (*(longlong *)(param_1 + 0x70) != 0) {
    thunk_FUN_1400e75f0();
  }
  local_188 = QString::fromAscii_helper("-restart",8);
  local_198 = (Data *)shared_null_exref;
  pQVar6 = (QString *)QCoreApplication::applicationFilePath();
  thunk_FUN_1400663e0(&local_198,&local_188);
  QProcess::startDetached(pQVar6,(QStringList *)&local_198);
  QString::~QString((QString *)&local_180);
  pDVar3 = local_198;
  if (*(int *)local_198 != 0) {
    if (*(int *)local_198 == -1) goto LAB_14008f3fc;
    LOCK();
    iVar2 = *(int *)local_198;
    *(int *)local_198 = *(int *)local_198 + -1;
    UNLOCK();
    if (iVar2 != 1) goto LAB_14008f3fc;
  }
  pQVar6 = (QString *)(local_198 + ((longlong)*(int *)(local_198 + 0xc) + 2) * 8);
  pQVar1 = (QString *)(local_198 + ((longlong)*(int *)(local_198 + 8) + 2) * 8);
  while (pQVar1 != pQVar6) {
    pQVar6 = pQVar6 + -8;
    QString::~QString(pQVar6);
  }
  QListData::dispose(pDVar3);
LAB_14008f3fc:
  QString::~QString((QString *)&local_188);
  return;
}

// =============================================================================
// Ghidra: FUN_14008fae0 @ 0x14008fae0
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14008fae0(longlong param_1)

{
  char cVar1;
  undefined4 uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  QString *this;
  QByteArray *this_00;
  char *pcVar5;
  ulonglong uVar6;
  undefined1 auStack_178 [32];
  undefined4 local_158;
  undefined ***local_148 [2];
  undefined4 local_138 [3];
  int iStack_12c;
  undefined *local_128;
  undefined **local_120;
  basic_ostream<char,std::char_traits<char>_> local_118 [96];
  undefined8 local_b8;
  undefined4 local_b0;
  basic_ios<char,std::char_traits<char>_> local_a0 [104];
  undefined8 local_38;
  ulonglong local_30 [3];
  ulonglong local_18;
  ulonglong local_10;
  
  local_38 = 0xfffffffffffffffe;
  local_10 = DAT_14050e9d0 ^ (ulonglong)auStack_178;
  local_138[0] = 0;
  uVar3 = thunk_FUN_140090080();
  cVar1 = thunk_FUN_1400d9660(uVar3);
  if (cVar1 == '\0') {
    if (*(char *)(param_1 + 0x79) != '\0') {
      thunk_FUN_1400e75f0(*(undefined8 *)(param_1 + 0x70));
      uVar3 = thunk_FUN_140076620();
      thunk_FUN_140101900(uVar3);
      *(undefined1 *)(param_1 + 0x79) = 0;
    }
    uVar3 = thunk_FUN_14018d250(local_148);
    cVar1 = thunk_FUN_14018ceb0(uVar3,20000);
    thunk_FUN_14018bd10(local_148);
    if (cVar1 != '\0') {
      local_128 = &DAT_14034ae60;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_a0);
      local_138[0] = 1;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_128,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_120,false);
      *(undefined ***)((longlong)&local_128 + (longlong)*(int *)(local_128 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_12c + (longlong)*(int *)(local_128 + 4)) =
           *(int *)(local_128 + 4) + -0x88;
      local_148[0] = &local_120;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_120);
      local_120 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_b8 = 0;
      local_b0 = 4;
      thunk_FUN_140042af0(&local_128,"time synchronization required for useful work");
      uVar3 = thunk_FUN_14007ce40(&local_128,local_30);
      uVar4 = thunk_FUN_14018d250(local_148);
      local_158 = 0x18b;
      thunk_FUN_14018c4e0(uVar4,20000,uVar3,
                          "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\app-grabber\\service.cpp");
      thunk_FUN_14018bd10(local_148);
      if (0xf < local_18) {
        uVar6 = local_30[0];
        if (0xfff < local_18 + 1) {
          if ((local_30[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar6 = *(ulonglong *)(local_30[0] - 8);
          if (local_30[0] <= uVar6) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_30[0] - uVar6 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_30[0] - uVar6) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002199(uVar6);
      }
      *(undefined ***)((longlong)&local_128 + (longlong)*(int *)(local_128 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_12c + (longlong)*(int *)(local_128 + 4)) =
           *(int *)(local_128 + 4) + -0x88;
      local_120 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140065bc0(&local_120);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_120);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_118);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_a0);
    }
  }
  else {
    uVar3 = thunk_FUN_140076760();
    cVar1 = thunk_FUN_1400a5de0(uVar3);
    if (cVar1 == '\0') {
      if (*(char *)(param_1 + 0x79) != '\0') {
        thunk_FUN_1400e75f0(*(undefined8 *)(param_1 + 0x70));
        uVar3 = thunk_FUN_140076620();
        thunk_FUN_140101900(uVar3);
        *(undefined1 *)(param_1 + 0x79) = 0;
      }
      uVar3 = thunk_FUN_14018d250(local_148);
      cVar1 = thunk_FUN_14018ceb0(uVar3,20000);
      thunk_FUN_14018bd10(local_148);
      if (cVar1 != '\0') {
        local_128 = &DAT_14034ae60;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_a0);
        local_138[0] = 2;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_128,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_120,false);
        *(undefined ***)((longlong)&local_128 + (longlong)*(int *)(local_128 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_12c + (longlong)*(int *)(local_128 + 4)) =
             *(int *)(local_128 + 4) + -0x88;
        thunk_FUN_140058f40(&local_120,2);
        uVar3 = thunk_FUN_140076760();
        uVar2 = thunk_FUN_1400a8ef0(uVar3);
        this = (QString *)thunk_FUN_140090120(local_148,uVar2);
        uVar3 = thunk_FUN_140042af0(&local_128,"passive work: ");
        this_00 = (QByteArray *)QString::toLocal8Bit(this);
        pcVar5 = QByteArray::data(this_00);
        thunk_FUN_140042af0(uVar3,pcVar5);
        QByteArray::~QByteArray((QByteArray *)local_138);
        QString::~QString((QString *)local_148);
        uVar3 = thunk_FUN_14007ce40(&local_128,local_30);
        uVar4 = thunk_FUN_14018d250(local_148);
        local_158 = 0x191;
        thunk_FUN_14018c4e0(uVar4,20000,uVar3,
                            "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\app-grabber\\service.cpp")
        ;
        thunk_FUN_14018bd10(local_148);
        if (0xf < local_18) {
          uVar6 = local_30[0];
          if (0xfff < local_18 + 1) {
            if ((local_30[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar6 = *(ulonglong *)(local_30[0] - 8);
            if (local_30[0] <= uVar6) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_30[0] - uVar6 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_30[0] - uVar6) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_140002199(uVar6);
        }
        thunk_FUN_14005cb60(local_a0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_a0);
      }
    }
    else if (*(char *)(param_1 + 0x79) == '\0') {
      uVar3 = thunk_FUN_140076620();
      uVar4 = thunk_FUN_140076760();
      thunk_FUN_1401029c0(uVar3,0,uVar4);
      thunk_FUN_1400e7210(*(undefined8 *)(param_1 + 0x70));
      *(undefined1 *)(param_1 + 0x79) = 1;
    }
  }
  return;
}

// =============================================================================
// Ghidra: FUN_1400902e0 @ 0x1400902e0
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1400902e0(int param_1,undefined8 param_2,QString *param_3)

{
  char cVar1;
  undefined8 uVar2;
  QByteArray *pQVar3;
  char *pcVar4;
  undefined8 uVar5;
  ulonglong uVar6;
  undefined1 auStack_168 [32];
  undefined4 local_148;
  undefined4 local_138 [3];
  int iStack_12c;
  undefined *local_128;
  basic_streambuf<char,std::char_traits<char>_> local_120 [128];
  basic_ios<char,std::char_traits<char>_> local_a0 [104];
  undefined8 local_38;
  ulonglong local_30 [3];
  ulonglong local_18;
  ulonglong local_10;
  
  local_38 = 0xfffffffffffffffe;
  local_10 = DAT_14050e9d0 ^ (ulonglong)auStack_168;
  local_138[0] = 0;
  if (param_1 == 0) {
    uVar2 = thunk_FUN_14018d250(local_138);
    cVar1 = thunk_FUN_14018ceb0(uVar2,10000);
    thunk_FUN_14018bd10(local_138);
    if (cVar1 == '\0') {
      return;
    }
    local_128 = &DAT_14034ae60;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_a0);
    local_138[0] = 1;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
    *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_12c + (longlong)*(int *)(local_128 + 4)) =
         *(int *)(local_128 + 4) + -0x88;
    thunk_FUN_140058f40(local_120,2);
    uVar2 = thunk_FUN_140042af0(&local_128,"qt say: [");
    pQVar3 = (QByteArray *)QString::toLocal8Bit(param_3);
    pcVar4 = QByteArray::data(pQVar3);
    thunk_FUN_140042af0(uVar2,pcVar4);
    QByteArray::~QByteArray((QByteArray *)local_138);
    thunk_FUN_140042af0(uVar2,&DAT_1403471b8);
    uVar2 = thunk_FUN_14007ce40(&local_128,local_30);
    uVar5 = thunk_FUN_14018d250(local_138);
    local_148 = 0x74;
    thunk_FUN_14018c4e0(uVar5,10000,uVar2,
                        "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\app-grabber\\service.cpp");
    thunk_FUN_14018bd10(local_138);
    if (0xf < local_18) {
      uVar6 = local_30[0];
      if (0xfff < local_18 + 1) {
        if ((local_30[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar6 = *(ulonglong *)(local_30[0] - 8);
        if (local_30[0] <= uVar6) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_30[0] - uVar6 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_30[0] - uVar6) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002199(uVar6);
    }
  }
  else if (param_1 == 1) {
    uVar2 = thunk_FUN_14018d250(local_138);
    cVar1 = thunk_FUN_14018ceb0(uVar2,30000);
    thunk_FUN_14018bd10(local_138);
    if (cVar1 == '\0') {
      return;
    }
    local_128 = &DAT_14034ae60;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_a0);
    local_138[0] = 2;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
    *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_12c + (longlong)*(int *)(local_128 + 4)) =
         *(int *)(local_128 + 4) + -0x88;
    thunk_FUN_140058f40(local_120,2);
    uVar2 = thunk_FUN_140042af0(&local_128,"qt say: [");
    pQVar3 = (QByteArray *)QString::toLocal8Bit(param_3);
    pcVar4 = QByteArray::data(pQVar3);
    thunk_FUN_140042af0(uVar2,pcVar4);
    QByteArray::~QByteArray((QByteArray *)local_138);
    thunk_FUN_140042af0(uVar2,&DAT_1403471b8);
    uVar2 = thunk_FUN_14007ce40(&local_128,local_30);
    uVar5 = thunk_FUN_14018d250(local_138);
    local_148 = 0x77;
    thunk_FUN_14018c4e0(uVar5,30000,uVar2,
                        "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\app-grabber\\service.cpp");
    thunk_FUN_14018bd10(local_138);
    if (0xf < local_18) {
      uVar6 = local_30[0];
      if (0xfff < local_18 + 1) {
        if ((local_30[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar6 = *(ulonglong *)(local_30[0] - 8);
        if (local_30[0] <= uVar6) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_30[0] - uVar6 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_30[0] - uVar6) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002199(uVar6);
    }
  }
  else if (param_1 == 2) {
    uVar2 = thunk_FUN_14018d250(local_138);
    cVar1 = thunk_FUN_14018ceb0(uVar2,40000);
    thunk_FUN_14018bd10(local_138);
    if (cVar1 == '\0') {
      return;
    }
    local_128 = &DAT_14034ae60;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_a0);
    local_138[0] = 4;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
    *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_12c + (longlong)*(int *)(local_128 + 4)) =
         *(int *)(local_128 + 4) + -0x88;
    thunk_FUN_140058f40(local_120,2);
    uVar2 = thunk_FUN_140042af0(&local_128,"qt say: [");
    pQVar3 = (QByteArray *)QString::toLocal8Bit(param_3);
    pcVar4 = QByteArray::data(pQVar3);
    thunk_FUN_140042af0(uVar2,pcVar4);
    QByteArray::~QByteArray((QByteArray *)local_138);
    thunk_FUN_140042af0(uVar2,&DAT_1403471b8);
    uVar2 = thunk_FUN_14007ce40(&local_128,local_30);
    uVar5 = thunk_FUN_14018d250(local_138);
    local_148 = 0x7a;
    thunk_FUN_14018c4e0(uVar5,40000,uVar2,
                        "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\app-grabber\\service.cpp");
    thunk_FUN_14018bd10(local_138);
    if (0xf < local_18) {
      uVar6 = local_30[0];
      if (0xfff < local_18 + 1) {
        if ((local_30[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar6 = *(ulonglong *)(local_30[0] - 8);
        if (local_30[0] <= uVar6) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_30[0] - uVar6 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_30[0] - uVar6) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002199(uVar6);
    }
  }
  else if (param_1 == 3) {
    uVar2 = thunk_FUN_14018d250(local_138);
    cVar1 = thunk_FUN_14018ceb0(uVar2,50000);
    thunk_FUN_14018bd10(local_138);
    if (cVar1 == '\0') {
      return;
    }
    local_128 = &DAT_14034ae60;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_a0);
    local_138[0] = 8;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
    *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_12c + (longlong)*(int *)(local_128 + 4)) =
         *(int *)(local_128 + 4) + -0x88;
    thunk_FUN_140058f40(local_120,2);
    uVar2 = thunk_FUN_140042af0(&local_128,"qt say: [");
    pQVar3 = (QByteArray *)QString::toLocal8Bit(param_3);
    pcVar4 = QByteArray::data(pQVar3);
    thunk_FUN_140042af0(uVar2,pcVar4);
    QByteArray::~QByteArray((QByteArray *)local_138);
    thunk_FUN_140042af0(uVar2,&DAT_1403471b8);
    uVar2 = thunk_FUN_14007ce40(&local_128,local_30);
    uVar5 = thunk_FUN_14018d250(local_138);
    local_148 = 0x7d;
    thunk_FUN_14018c4e0(uVar5,50000,uVar2,
                        "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\app-grabber\\service.cpp");
    thunk_FUN_14018bd10(local_138);
    if (0xf < local_18) {
      uVar6 = local_30[0];
      if (0xfff < local_18 + 1) {
        if ((local_30[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar6 = *(ulonglong *)(local_30[0] - 8);
        if (local_30[0] <= uVar6) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_30[0] - uVar6 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_30[0] - uVar6) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002199(uVar6);
    }
  }
  else {
    uVar2 = thunk_FUN_14018d250(local_138);
    cVar1 = thunk_FUN_14018ceb0(uVar2,20000);
    thunk_FUN_14018bd10(local_138);
    if (cVar1 == '\0') {
      return;
    }
    local_128 = &DAT_14034ae60;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_a0);
    local_138[0] = 0x10;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
    *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_12c + (longlong)*(int *)(local_128 + 4)) =
         *(int *)(local_128 + 4) + -0x88;
    thunk_FUN_140058f40(local_120,2);
    uVar2 = thunk_FUN_140042af0(&local_128,"qt say: [");
    pQVar3 = (QByteArray *)QString::toLocal8Bit(param_3);
    pcVar4 = QByteArray::data(pQVar3);
    thunk_FUN_140042af0(uVar2,pcVar4);
    QByteArray::~QByteArray((QByteArray *)local_138);
    thunk_FUN_140042af0(uVar2,&DAT_1403471b8);
    uVar2 = thunk_FUN_14007ce40(&local_128,local_30);
    uVar5 = thunk_FUN_14018d250(local_138);
    local_148 = 0x80;
    thunk_FUN_14018c4e0(uVar5,20000,uVar2,
                        "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\app-grabber\\service.cpp");
    thunk_FUN_14018bd10(local_138);
    if (0xf < local_18) {
      uVar6 = local_30[0];
      if (0xfff < local_18 + 1) {
        if ((local_30[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar6 = *(ulonglong *)(local_30[0] - 8);
        if (local_30[0] <= uVar6) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_30[0] - uVar6 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_30[0] - uVar6) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002199(uVar6);
    }
  }
  thunk_FUN_14005cb60(local_a0);
  std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_a0);
  return;
}

// =============================================================================
// Ghidra: FUN_140282230 @ 0x140282230
// =============================================================================
undefined8 *
FUN_140282230(undefined8 *param_1,undefined8 *param_2,undefined8 param_3,undefined8 param_4)

{
  undefined8 uVar1;
  longlong lVar2;
  QTypedArrayData<unsigned_short> *local_res18;
  QTypedArrayData<unsigned_short> *local_98;
  Connection local_90 [8];
  Connection local_88 [8];
  undefined8 local_80;
  code *local_78;
  int *piStack_70;
  undefined4 local_68;
  undefined4 uStack_64;
  undefined4 uStack_60;
  undefined4 uStack_5c;
  undefined4 local_58;
  undefined4 uStack_54;
  undefined4 uStack_50;
  undefined4 uStack_4c;
  undefined1 local_48 [32];
  
  local_80 = 0xfffffffffffffffe;
  local_98 = QString::fromAscii_helper("video",5);
  local_res18 = QString::fromAscii_helper("videoRouter4Grabber",0x13);
  local_78 = (code *)*param_2;
  piStack_70 = (int *)param_2[1];
  if (piStack_70 != (int *)0x0) {
    LOCK();
    *piStack_70 = *piStack_70 + 1;
    UNLOCK();
    LOCK();
    piStack_70[1] = piStack_70[1] + 1;
    UNLOCK();
  }
  thunk_FUN_140281780(param_1,&local_res18,&local_98,&local_78);
  QString::~QString((QString *)&local_res18);
  QString::~QString((QString *)&local_98);
  *param_1 = grabber::VideoSink::vftable;
  param_1[0x36] = 0;
  param_1[0x37] = param_3;
  param_1[0x39] = shared_null_exref;
  QTimer::QTimer((QTimer *)(param_1 + 0x3a),(QObject *)0x0);
  param_1[0x3e] = shared_null_exref;
  param_1[0x3f] = shared_null_exref;
  param_1[0x40] = shared_null_exref;
  param_1[0x41] = shared_null_exref;
  param_1[0x42] = shared_null_exref;
  uVar1 = thunk_FUN_140076760();
  lVar2 = thunk_FUN_1400a4ad0(uVar1,local_48);
  param_1[0x36] = *(undefined8 *)(lVar2 + 8);
  thunk_FUN_14028cfc0(param_1,param_4);
  *(uint *)((longlong)param_1 + 0x1ec) = *(uint *)((longlong)param_1 + 0x1ec) & 0xfffffffe;
  local_78 = thunk_FUN_140289070;
  uStack_5c = (undefined4)((ulonglong)piStack_70 >> 0x20);
  piStack_70 = (int *)((ulonglong)piStack_70 & 0xffffffff00000000);
  local_68 = 0x40013ce6;
  uStack_64 = 1;
  uStack_60 = 0;
  thunk_FUN_14027f240(local_90,param_1 + 0x3a,timeout_exref,param_1,&local_68,0);
  QMetaObject::Connection::~Connection(local_90);
  local_78 = (code *)&LAB_14000a29a;
  uStack_4c = (undefined4)((ulonglong)piStack_70 >> 0x20);
  piStack_70 = (int *)((ulonglong)piStack_70 & 0xffffffff00000000);
  local_58 = 0x4000a29a;
  uStack_54 = 1;
  uStack_50 = 0;
  thunk_FUN_14027f330(local_88,param_1[0x31],thunk_FUN_1401f3910,param_1,&local_58,0);
  QMetaObject::Connection::~Connection(local_88);
  thunk_FUN_1400e4d10(param_2[1]);
  return param_1;
}

// =============================================================================
// Ghidra: FUN_140289760 @ 0x140289760
// =============================================================================
void FUN_140289760(longlong param_1,QString *param_2,longlong *param_3)

{
  longlong *plVar1;
  longlong lVar2;
  bool bVar3;
  QMapNodeBase *this;
  QDateTime local_res8 [8];
  QString *local_res10;
  longlong *local_res18;
  
  local_res10 = param_2;
  local_res18 = param_3;
  if (*param_3 == 0) {
    qt_assert("!screenshoot.isNull()",
              "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabber-video\\videoSink.cpp",0x124
             );
  }
  thunk_FUN_1402a30a0(local_res8);
  plVar1 = (longlong *)(param_1 + 0x1c8);
  if (1 < *(uint *)*plVar1) {
    thunk_FUN_140286cc0(plVar1);
  }
  lVar2 = *plVar1;
  if (*(longlong *)(lVar2 + 0x10) == 0) {
    this = (QMapNodeBase *)(lVar2 + 8);
  }
  else {
    this = *(QMapNodeBase **)(lVar2 + 0x20);
  }
  if (1 < *(uint *)*plVar1) {
    thunk_FUN_140286cc0(plVar1);
  }
  lVar2 = *plVar1;
  for (; this != (QMapNodeBase *)(lVar2 + 8); this = QMapNodeBase::nextNode(this)) {
    bVar3 = operator==((QString *)(this + 0x20),param_2);
    if ((((bVar3) && (*(int *)(this + 0x30) == 2 - (uint)(((QString *)*param_3)[8] != (QString)0x0))
         ) && (bVar3 = operator==((QString *)(this + 0x28),(QString *)*param_3), bVar3)) &&
       (*(int *)(this + 0x34) == *(int *)(*param_3 + 0xc))) {
      thunk_FUN_14028ec30(*(undefined8 *)(this + 0x50),local_res8);
      thunk_FUN_14028c570(param_1,local_res8,this + 0x18,this + 0x40);
    }
  }
  QDateTime::~QDateTime(local_res8);
  QString::~QString(param_2);
  thunk_FUN_1400e45c0(param_3[1]);
  return;
}

// =============================================================================
// Ghidra: FUN_14028acd0 @ 0x14028acd0
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14028acd0(longlong param_1,longlong *param_2)

{
  longlong *plVar1;
  char cVar2;
  undefined8 uVar3;
  basic_ostream<char,std::char_traits<char>_> *pbVar4;
  basic_ostream<char,struct_std::char_traits<char>_> *pbVar5;
  undefined8 uVar6;
  longlong lVar7;
  QString *this;
  QByteArray *this_00;
  char *pcVar8;
  ulonglong uVar9;
  undefined1 auStack_198 [32];
  undefined4 local_178;
  undefined ***local_168 [2];
  undefined4 local_158 [3];
  int iStack_14c;
  undefined *local_148;
  undefined **local_140;
  basic_ostream<char,std::char_traits<char>_> local_138 [96];
  undefined8 local_d8;
  undefined4 local_d0;
  basic_ios<char,std::char_traits<char>_> local_c0 [104];
  undefined8 local_58;
  ulonglong local_50 [3];
  ulonglong local_38;
  ulonglong local_30;
  
  local_58 = 0xfffffffffffffffe;
  local_30 = DAT_14050e9d0 ^ (ulonglong)auStack_198;
  local_158[0] = 0;
  lVar7 = *param_2;
  if (*(longlong *)(lVar7 + 8) == *(longlong *)(param_1 + 0x1b0)) {
    plVar1 = (longlong *)(param_1 + 0x1c8);
    if (1 < *(uint *)*plVar1) {
      thunk_FUN_140286cc0(plVar1);
    }
    lVar7 = thunk_FUN_140287b00(*plVar1,lVar7 + 8);
    if (lVar7 == 0) {
      lVar7 = *plVar1 + 8;
    }
    if (1 < *(uint *)*plVar1) {
      thunk_FUN_140286cc0(plVar1);
    }
    if (*plVar1 + 8 == lVar7) {
      uVar3 = thunk_FUN_14018d250(local_168);
      cVar2 = thunk_FUN_14018ceb0(uVar3,30000);
      thunk_FUN_14018bd10(local_168);
      if (cVar2 != '\0') {
        local_148 = &DAT_14034ae60;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_c0);
        local_158[0] = 2;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_148,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_140,false);
        *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
             *(int *)(local_148 + 4) + -0x88;
        thunk_FUN_140058f40(&local_140,2);
        this = (QString *)thunk_FUN_140080e80(*param_2 + 8,local_168);
        uVar3 = thunk_FUN_140042af0(&local_148,"absent stream frame response effort: ");
        this_00 = (QByteArray *)QString::toLocal8Bit(this);
        pcVar8 = QByteArray::data(this_00);
        thunk_FUN_140042af0(uVar3,pcVar8);
        QByteArray::~QByteArray((QByteArray *)local_158);
        QString::~QString((QString *)local_168);
        uVar3 = thunk_FUN_14007ce40(&local_148,local_50);
        uVar6 = thunk_FUN_14018d250(local_168);
        local_178 = 0x8a;
        thunk_FUN_14018c4e0(uVar6,30000,uVar3,
                            "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabber-video\\videoSink.cpp"
                           );
        thunk_FUN_14018bd10(local_168);
        if (0xf < local_38) {
          uVar9 = local_50[0];
          if (0xfff < local_38 + 1) {
            if ((local_50[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar9 = *(ulonglong *)(local_50[0] - 8);
            if (local_50[0] <= uVar9) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_50[0] - uVar9 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_50[0] - uVar9) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_140002199(uVar9);
        }
        thunk_FUN_14005cb60(local_c0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_c0);
      }
    }
    else {
      if (*(longlong *)(lVar7 + 0x50) != 0) {
        thunk_FUN_140290b40(*(longlong *)(lVar7 + 0x50),*param_2 + 0x30);
      }
      uVar3 = thunk_FUN_1402a30a0(local_168);
      thunk_FUN_14028c570(param_1,uVar3,*param_2 + 8,lVar7 + 0x40);
      QDateTime::~QDateTime((QDateTime *)local_168);
    }
  }
  else {
    uVar3 = thunk_FUN_14018d250(local_168);
    cVar2 = thunk_FUN_14018ceb0(uVar3,30000);
    thunk_FUN_14018bd10(local_168);
    if (cVar2 != '\0') {
      local_148 = &DAT_14034ae60;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_c0);
      local_158[0] = 1;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_148,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_140,false);
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      local_168[0] = &local_140;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_d8 = 0;
      local_d0 = 4;
      lVar7 = *param_2;
      pbVar4 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_140042af0(&local_148,"alien grabber stream frame response effort, this ");
      pbVar5 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                         (pbVar4,*(__int64 *)(param_1 + 0x1b0));
      pbVar4 = (basic_ostream<char,std::char_traits<char>_> *)thunk_FUN_140042af0(pbVar5,", try ");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar4,*(__int64 *)(lVar7 + 8));
      uVar3 = thunk_FUN_14007ce40(&local_148,local_50);
      uVar6 = thunk_FUN_14018d250(local_168);
      local_178 = 0x82;
      thunk_FUN_14018c4e0(uVar6,30000,uVar3,
                          "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabber-video\\videoSink.cpp"
                         );
      thunk_FUN_14018bd10(local_168);
      if (0xf < local_38) {
        uVar9 = local_50[0];
        if (0xfff < local_38 + 1) {
          if ((local_50[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar9 = *(ulonglong *)(local_50[0] - 8);
          if (local_50[0] <= uVar9) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_50[0] - uVar9 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_50[0] - uVar9) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002199(uVar9);
      }
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140065bc0(&local_140);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_138);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_c0);
    }
  }
  return;
}

// =============================================================================
// Ghidra: FUN_14028b2c0 @ 0x14028b2c0
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14028b2c0(longlong param_1,longlong *param_2)

{
  longlong *plVar1;
  double dVar2;
  longlong *plVar3;
  double dVar4;
  double dVar5;
  longlong *plVar6;
  char cVar7;
  bool bVar8;
  undefined1 uVar9;
  int iVar10;
  void **ppvVar11;
  undefined8 uVar12;
  basic_ostream<char,std::char_traits<char>_> *pbVar13;
  basic_ostream<char,struct_std::char_traits<char>_> *pbVar14;
  undefined8 uVar15;
  QByteArray *this;
  char *pcVar16;
  undefined8 *puVar17;
  longlong lVar18;
  undefined4 *puVar19;
  undefined8 *puVar20;
  double *pdVar21;
  ulonglong uVar22;
  QListData *pQVar23;
  longlong lVar24;
  uint uVar25;
  undefined ***pppuVar26;
  longlong lVar27;
  int iVar28;
  undefined1 auStack_398 [32];
  undefined4 local_378;
  uint local_368;
  longlong local_360;
  undefined4 *local_358;
  longlong *local_350;
  undefined ***local_348;
  QString local_340 [8];
  undefined1 local_338;
  undefined4 local_334;
  undefined8 local_330;
  int *local_328;
  QString local_320 [8];
  undefined1 local_318;
  undefined4 local_314;
  QByteArray local_310 [8];
  QDateTime local_308 [8];
  undefined1 local_300 [8];
  undefined4 *local_2f8;
  undefined8 local_2f0;
  void *local_2e8;
  undefined8 local_2e0;
  undefined1 local_2d8 [16];
  undefined1 local_2c8 [16];
  undefined1 local_2b8 [16];
  undefined1 local_2a8 [12];
  int iStack_29c;
  undefined *local_298;
  undefined **local_290;
  basic_ostream<char,std::char_traits<char>_> local_288 [96];
  undefined8 local_228;
  undefined4 local_220;
  basic_ios<char,std::char_traits<char>_> local_210 [100];
  int iStack_1ac;
  undefined *local_1a8;
  undefined **local_1a0;
  basic_ostream<char,std::char_traits<char>_> local_198 [96];
  undefined8 local_138;
  undefined4 local_130;
  basic_ios<char,std::char_traits<char>_> local_120 [104];
  ulonglong local_b8 [3];
  ulonglong local_a0;
  ulonglong local_98 [3];
  ulonglong local_80;
  ulonglong local_78;
  
  local_2f0 = 0xfffffffffffffffe;
  local_78 = DAT_14050e9d0 ^ (ulonglong)auStack_398;
  uVar25 = 0;
  local_368 = 0;
  iVar28 = 0;
  local_360 = param_1;
  local_350 = param_2;
  iVar10 = QListData::size((QListData *)(*param_2 + 8));
  dVar5 = DAT_1404492d8;
  dVar4 = DAT_14042cfe0;
  if (0 < iVar10) {
    do {
      pQVar23 = (QListData *)(*param_2 + 8);
      if ((iVar28 < 0) || (iVar10 = QListData::size(pQVar23), iVar10 <= iVar28)) {
        qt_assert_x("QList<T>::operator[]","index out of range",
                    "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",0x21b);
      }
      if (1 < **(uint **)pQVar23) {
        thunk_FUN_14006f9b0(pQVar23,(*(uint **)pQVar23)[1]);
      }
      ppvVar11 = QListData::at(pQVar23,iVar28);
      plVar3 = *ppvVar11;
      if (*plVar3 == *(longlong *)(param_1 + 0x1b0)) {
        plVar1 = (longlong *)(param_1 + 0x1f0);
        if (1 < *(uint *)*plVar1) {
          thunk_FUN_1400f9190(plVar1);
        }
        if (*(longlong *)(*plVar1 + 0x10) == 0) {
LAB_14028b5fc:
          lVar27 = *plVar1 + 8;
        }
        else {
          lVar27 = 0;
          lVar18 = *(longlong *)(*plVar1 + 0x10);
          do {
            bVar8 = operator<((QString *)(lVar18 + 0x18),(QString *)(plVar3 + 1));
            if (bVar8) {
              lVar24 = *(longlong *)(lVar18 + 0x10);
            }
            else {
              lVar24 = *(longlong *)(lVar18 + 8);
              lVar27 = lVar18;
            }
            lVar18 = lVar24;
          } while (lVar24 != 0);
          if ((lVar27 == 0) ||
             (bVar8 = operator<((QString *)(plVar3 + 1),(QString *)(lVar27 + 0x18)), bVar8))
          goto LAB_14028b5fc;
        }
        if (1 < *(uint *)*plVar1) {
          thunk_FUN_1400f9190(plVar1);
        }
        if (*plVar1 + 8 == lVar27) {
          uVar12 = thunk_FUN_14018d250(local_2b8);
          cVar7 = thunk_FUN_14018ceb0(uVar12,30000);
          thunk_FUN_14018bd10(local_2b8);
          param_1 = local_360;
          if (cVar7 != '\0') {
            local_1a8 = &DAT_14034ae60;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_120);
            local_368 = uVar25 | 2;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1a8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1a0,false);
            *(undefined ***)((longlong)&local_1a8 + (longlong)*(int *)(local_1a8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1ac + (longlong)*(int *)(local_1a8 + 4)) =
                 *(int *)(local_1a8 + 4) + -0x88;
            local_348 = &local_1a0;
            std::basic_streambuf<char,std::char_traits<char>_>::
            basic_streambuf<char,std::char_traits<char>_>
                      ((basic_streambuf<char,std::char_traits<char>_> *)&local_1a0);
            local_1a0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                        vftable;
            local_138 = 0;
            local_130 = 4;
            uVar12 = thunk_FUN_140042af0(&local_1a8,"alien session stream selection effort, ");
            this = (QByteArray *)QString::toLocal8Bit((QString *)(plVar3 + 1));
            pcVar16 = QByteArray::data(this);
            thunk_FUN_140042af0(uVar12,pcVar16);
            QByteArray::~QByteArray(local_310);
            uVar12 = thunk_FUN_14007ce40(&local_1a8,local_98);
            uVar15 = thunk_FUN_14018d250(local_2a8);
            local_378 = 0x32;
            thunk_FUN_14018c4e0(uVar15,30000,uVar12,
                                "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabber-video\\videoSink.cpp"
                               );
            thunk_FUN_14018bd10(local_2a8);
            if (0xf < local_80) {
              uVar22 = local_98[0];
              if (0xfff < local_80 + 1) {
                if ((local_98[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
                uVar22 = *(ulonglong *)(local_98[0] - 8);
                if (local_98[0] <= uVar22) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
                if (local_98[0] - uVar22 < 8) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
                if (0x27 < local_98[0] - uVar22) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
              }
              FUN_140002199(uVar22);
            }
            *(undefined ***)((longlong)&local_1a8 + (longlong)*(int *)(local_1a8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1ac + (longlong)*(int *)(local_1a8 + 4)) =
                 *(int *)(local_1a8 + 4) + -0x88;
            local_1a0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                        vftable;
            thunk_FUN_140065bc0(&local_1a0);
            std::basic_streambuf<char,std::char_traits<char>_>::
            ~basic_streambuf<char,std::char_traits<char>_>
                      ((basic_streambuf<char,std::char_traits<char>_> *)&local_1a0);
            std::basic_ostream<char,std::char_traits<char>_>::
            ~basic_ostream<char,std::char_traits<char>_>(local_198);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_120);
            param_1 = local_360;
            uVar25 = uVar25 | 2;
          }
        }
        else {
          pppuVar26 = (undefined ***)(local_360 + 0x1c8);
          local_348 = pppuVar26;
          puVar17 = (undefined8 *)thunk_FUN_1402836a0(pppuVar26,plVar3);
          if (puVar17[2] == 0) {
            local_2e8 = operator_new(0xb8);
            if (local_2e8 == (void *)0x0) {
              lVar18 = 0;
            }
            else {
              lVar18 = thunk_FUN_14028e700(local_2e8,*(undefined4 *)((longlong)plVar3 + 0x1c));
            }
            if (lVar18 == 0) {
              puVar19 = (undefined4 *)0x0;
              local_2f8 = (undefined4 *)0x0;
            }
            else {
              puVar19 = operator_new(0x18);
              if ((longlong *)(puVar19 + 4) != (longlong *)0x0) {
                *(longlong *)(puVar19 + 4) = lVar18;
              }
              if (puVar19 != (undefined4 *)0x0) {
                *(undefined1 **)(puVar19 + 2) = &LAB_1400094bc;
                puVar19[1] = 1;
                *puVar19 = 1;
              }
              local_358 = puVar19;
              local_2f8 = puVar19;
              thunk_FUN_14007c8c0(puVar19,lVar18,1);
              FUN_140002054(local_300,lVar18);
            }
            local_358 = (undefined4 *)puVar17[3];
            puVar17[3] = puVar19;
            puVar17[2] = lVar18;
            thunk_FUN_140286560();
            thunk_FUN_140286560(0);
            puVar17[4] = 0;
            puVar20 = (undefined8 *)thunk_FUN_140283800(plVar1,plVar3 + 1);
            local_330 = *puVar20;
            local_328 = (int *)puVar20[1];
            if (local_328 != (int *)0x0) {
              LOCK();
              *local_328 = *local_328 + 1;
              UNLOCK();
              LOCK();
              local_328[1] = local_328[1] + 1;
              UNLOCK();
            }
            local_2e0 = puVar17[1];
            puVar17[1] = local_328;
            *puVar17 = local_330;
            thunk_FUN_1400e4b70();
            pppuVar26 = local_348;
          }
          pQVar23 = (QListData *)(*local_350 + 0x10);
          if ((iVar28 < 0) || (iVar10 = QListData::size(pQVar23), iVar10 <= iVar28)) {
            qt_assert_x("QList<T>::operator[]","index out of range",
                        "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",0x21b
                       );
          }
          if (1 < **(uint **)pQVar23) {
            thunk_FUN_14006f8f0(pQVar23,(*(uint **)pQVar23)[1]);
          }
          ppvVar11 = QListData::at(pQVar23,iVar28);
          plVar6 = local_350;
          if ((double)*ppvVar11 <= 0.0) {
            if ((*(int *)((longlong)plVar3 + 0x1c) == -1) && ((int)plVar3[3] == 1)) {
              puVar17[4] = 0;
              thunk_FUN_14028f800(puVar17[2]);
            }
            else {
              thunk_FUN_14028bed0(pppuVar26,plVar3);
            }
            QString::QString(local_340);
            local_338 = (int)plVar3[3] == 1;
            QString::operator=(local_340,(QString *)(plVar3 + 2));
            local_334 = *(undefined4 *)((longlong)plVar3 + 0x1c);
            puVar17 = (undefined8 *)thunk_FUN_140283800(plVar1,plVar3 + 1);
            plVar3 = (longlong *)*puVar17;
            QString::QString(local_320,local_340);
            local_318 = local_338;
            local_314 = local_334;
            (**(code **)(*plVar3 + 0xc0))(plVar3,local_320);
            QString::~QString(local_340);
            param_1 = local_360;
            uVar25 = local_368;
          }
          else {
            pQVar23 = (QListData *)(*local_350 + 0x10);
            if ((iVar28 < 0) || (iVar10 = QListData::size(pQVar23), iVar10 <= iVar28)) {
              qt_assert_x("QList<T>::operator[]","index out of range",
                          "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",
                          0x21b);
            }
            if (1 < **(uint **)pQVar23) {
              thunk_FUN_14006f8f0(pQVar23,(*(uint **)pQVar23)[1]);
            }
            ppvVar11 = QListData::at(pQVar23,iVar28);
            if ((double)puVar17[4] != (double)*ppvVar11) {
              pdVar21 = (double *)thunk_FUN_140283520(*plVar6 + 0x10,iVar28);
              dVar2 = *pdVar21;
              puVar17[4] = dVar2;
              if ((char)plVar3[4] != '\0') {
                thunk_FUN_140291980(puVar17[2],(int)(dVar2 * dVar4 + dVar5));
                uVar12 = puVar17[2];
                uVar9 = thunk_FUN_1400683c0(local_360 + 0x210,plVar3 + 1);
                thunk_FUN_140291a80(uVar12,uVar9);
              }
            }
            uVar12 = thunk_FUN_1402a30a0(local_308);
            param_1 = local_360;
            thunk_FUN_14028c570(local_360,uVar12,plVar3,puVar17);
            QDateTime::~QDateTime(local_308);
            uVar25 = local_368;
          }
        }
      }
      else {
        uVar12 = thunk_FUN_14018d250(local_2d8);
        cVar7 = thunk_FUN_14018ceb0(uVar12,30000);
        thunk_FUN_14018bd10(local_2d8);
        if (cVar7 != '\0') {
          local_298 = &DAT_14034ae60;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_210);
          local_368 = uVar25 | 1;
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_298,
                     (basic_streambuf<char,std::char_traits<char>_> *)&local_290,false);
          *(undefined ***)
           ((basic_ostream<char,std::char_traits<char>_> *)&local_298 + *(int *)(local_298 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_29c + (longlong)*(int *)(local_298 + 4)) =
               *(int *)(local_298 + 4) + -0x88;
          local_348 = &local_290;
          std::basic_streambuf<char,std::char_traits<char>_>::
          basic_streambuf<char,std::char_traits<char>_>
                    ((basic_streambuf<char,std::char_traits<char>_> *)&local_290);
          local_290 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                      vftable;
          local_228 = 0;
          local_220 = 4;
          pbVar13 = (basic_ostream<char,std::char_traits<char>_> *)
                    thunk_FUN_140042af0((basic_ostream<char,std::char_traits<char>_> *)&local_298,
                                        "alien grabber stream selection effort, this ");
          pbVar14 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                              (pbVar13,*(__int64 *)(param_1 + 0x1b0));
          pbVar13 = (basic_ostream<char,std::char_traits<char>_> *)
                    thunk_FUN_140042af0(pbVar14,", try ");
          std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar13,*plVar3);
          uVar12 = thunk_FUN_14007ce40((basic_ostream<char,std::char_traits<char>_> *)&local_298,
                                       local_b8);
          uVar15 = thunk_FUN_14018d250(local_2c8);
          local_378 = 0x2b;
          thunk_FUN_14018c4e0(uVar15,30000,uVar12,
                              "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabber-video\\videoSink.cpp"
                             );
          thunk_FUN_14018bd10(local_2c8);
          if (0xf < local_a0) {
            uVar22 = local_b8[0];
            if (0xfff < local_a0 + 1) {
              if ((local_b8[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              uVar22 = *(ulonglong *)(local_b8[0] - 8);
              if (local_b8[0] <= uVar22) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (local_b8[0] - uVar22 < 8) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (0x27 < local_b8[0] - uVar22) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
            }
            FUN_140002199(uVar22);
          }
          *(undefined ***)
           ((basic_ostream<char,std::char_traits<char>_> *)&local_298 + *(int *)(local_298 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_29c + (longlong)*(int *)(local_298 + 4)) =
               *(int *)(local_298 + 4) + -0x88;
          local_290 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                      vftable;
          thunk_FUN_140065bc0(&local_290);
          std::basic_streambuf<char,std::char_traits<char>_>::
          ~basic_streambuf<char,std::char_traits<char>_>
                    ((basic_streambuf<char,std::char_traits<char>_> *)&local_290);
          std::basic_ostream<char,std::char_traits<char>_>::
          ~basic_ostream<char,std::char_traits<char>_>(local_288);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_210);
          uVar25 = uVar25 | 1;
        }
      }
      param_2 = local_350;
      iVar28 = iVar28 + 1;
      iVar10 = QListData::size((QListData *)(*local_350 + 8));
    } while (iVar28 < iVar10);
  }
  thunk_FUN_14028db10(param_1);
  return;
}

// =============================================================================
// Ghidra: FUN_14028c570 @ 0x14028c570
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14028c570(longlong param_1,undefined8 param_2,longlong param_3,longlong param_4)

{
  char cVar1;
  bool bVar2;
  undefined8 uVar3;
  QString *this;
  QByteArray *this_00;
  char *pcVar4;
  undefined8 uVar5;
  ulonglong uVar6;
  undefined1 auStack_1c8 [32];
  undefined4 local_1a8;
  QDateTime local_198 [8];
  undefined4 local_190 [2];
  longlong local_188;
  undefined8 local_180;
  QString local_178 [16];
  QDateTime local_168 [8];
  undefined8 local_160;
  undefined *local_158;
  basic_streambuf<char,std::char_traits<char>_> local_150 [128];
  basic_ios<char,std::char_traits<char>_> local_d0 [104];
  ulonglong local_68;
  QString local_60 [8];
  QString local_58 [8];
  ulonglong local_50;
  QDateTime local_40 [8];
  ulonglong local_38;
  
  local_160 = 0xfffffffffffffffe;
  local_38 = DAT_14050e9d0 ^ (ulonglong)auStack_1c8;
  local_190[0] = 0;
  if (((0.0 < *(double *)(param_4 + 0x20)) && (*(longlong *)(param_4 + 0x10) != 0)) &&
     (cVar1 = thunk_FUN_140290de0(), cVar1 != '\0')) {
    thunk_FUN_14028ee20(*(undefined8 *)(param_4 + 0x10),&local_188);
    if (local_188 != 0) {
      uVar3 = QDateTime::QDateTime(local_168);
      thunk_FUN_14028e230(param_1 + 0x1f8,local_198,param_3 + 8,uVar3);
      QDateTime::~QDateTime(local_168);
      bVar2 = QDateTime::isValid(local_198);
      if ((!bVar2) ||
         (bVar2 = QDateTime::operator<((QDateTime *)(local_188 + 0x30),local_198), bVar2)) {
        uVar3 = thunk_FUN_14018d250(local_178);
        cVar1 = thunk_FUN_14018ceb0(uVar3,30000);
        thunk_FUN_14018bd10(local_178);
        if (cVar1 != '\0') {
          local_158 = &DAT_14034ae60;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_d0);
          local_190[0] = 1;
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_158,local_150,false);
          *(undefined ***)(local_150 + (longlong)*(int *)(local_158 + 4) + -8) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&local_160 + (longlong)*(int *)(local_158 + 4) + 4) =
               *(int *)(local_158 + 4) + -0x88;
          thunk_FUN_140058f40(local_150,2);
          this = (QString *)thunk_FUN_140080e80(param_3,local_178);
          uVar3 = thunk_FUN_140042af0(&local_158,
                                      "trying to send frame while time is not negotiated: ");
          uVar3 = thunk_FUN_1400a1260(uVar3,local_198);
          uVar3 = thunk_FUN_140042af0(uVar3,&DAT_14034cc24);
          uVar3 = thunk_FUN_1400a1260(uVar3,local_188 + 0x30);
          uVar3 = thunk_FUN_140042af0(uVar3," for ");
          this_00 = (QByteArray *)QString::toLocal8Bit(this);
          pcVar4 = QByteArray::data(this_00);
          thunk_FUN_140042af0(uVar3,pcVar4);
          QByteArray::~QByteArray((QByteArray *)local_190);
          QString::~QString(local_178);
          uVar3 = thunk_FUN_14007ce40(&local_158,&local_68);
          uVar5 = thunk_FUN_14018d250(local_178);
          local_1a8 = 0x1de;
          thunk_FUN_14018c4e0(uVar5,30000,uVar3,
                              "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabber-video\\videoSink.cpp"
                             );
          thunk_FUN_14018bd10(local_178);
          if (0xf < local_50) {
            uVar6 = local_68;
            if (0xfff < local_50 + 1) {
              if ((local_68 & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              uVar6 = *(ulonglong *)(local_68 - 8);
              if (local_68 <= uVar6) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (local_68 - uVar6 < 8) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (0x27 < local_68 - uVar6) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
            }
            FUN_140002199(uVar6);
          }
          thunk_FUN_14005cb60(local_d0);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_d0);
        }
        thunk_FUN_14028f800(*(undefined8 *)(param_4 + 0x10));
      }
      else {
        uVar3 = thunk_FUN_140280120(&local_68,param_3,local_188 + 0x30);
        thunk_FUN_140288210(param_1 + 0x208,local_178,param_3 + 0x10,uVar3);
        QDateTime::~QDateTime(local_40);
        QString::~QString(local_58);
        QString::~QString(local_60);
        thunk_FUN_140283170(local_188 + 8,param_3);
        thunk_FUN_140280bf0(param_1,&local_188);
      }
      QDateTime::~QDateTime(local_198);
    }
    thunk_FUN_140286490(local_180);
  }
  return;
}

// =============================================================================
// Ghidra: FUN_14028ee20 @ 0x14028ee20
// =============================================================================
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 * FUN_14028ee20(longlong param_1,undefined8 *param_2)

{
  bool bVar1;
  Format FVar2;
  int iVar3;
  int iVar4;
  QDateTime *pQVar5;
  QArrayData *pQVar6;
  QArrayData *pQVar7;
  undefined8 uVar8;
  QRect *this;
  QRect *pQVar9;
  void *pvVar10;
  void *pvVar11;
  QArrayData **ppQVar12;
  QArrayData *pQVar13;
  QArrayData *pQVar14;
  uint uVar15;
  QArrayData *local_res8;
  undefined8 *local_res10;
  QArrayData **local_res18;
  QArrayData *local_108;
  QArrayData *local_100;
  QDateTime local_f8 [8];
  QImage local_f0 [32];
  QDateTime local_d0;
  QArrayData *local_c8;
  QArrayData *local_c0;
  QArrayData *local_b8;
  QArrayData *local_b0;
  QArrayData *local_a8;
  QArrayData *local_a0;
  QArrayData *local_98;
  QArrayData *local_90;
  undefined4 local_88;
  undefined8 local_80;
  QRect local_78 [64];
  
  local_80 = 0xfffffffffffffffe;
  pQVar13 = (QArrayData *)0x0;
  uVar15 = 0;
  local_88 = 0;
  if (*(char *)(param_1 + 0x30) != '\0') {
    *param_2 = 0;
    param_2[1] = 0;
    return param_2;
  }
  local_res10 = param_2;
  QDateTime::QDateTime(local_f8);
  QImage::QImage(local_f0);
  if (*(int *)(param_1 + 0x2c) == 0) {
    *param_2 = 0;
    param_2[1] = 0;
    goto LAB_14028f25d;
  }
  pQVar5 = (QDateTime *)thunk_FUN_14028eb30(param_1 + 0x20,*(undefined4 *)(param_1 + 0x28));
  QDateTime::operator=(local_f8,pQVar5);
  QImage::operator=(local_f0,(QImage *)(pQVar5 + 8));
  local_d0 = pQVar5[0x28];
  *(int *)(param_1 + 0x28) = (*(int *)(param_1 + 0x28) + 1) % *(int *)(param_1 + 0x18);
  *(int *)(param_1 + 0x2c) = *(int *)(param_1 + 0x2c) + -1;
  local_res8 = operator_new(0x58);
  pQVar6 = pQVar13;
  if (local_res8 != (QArrayData *)0x0) {
    pQVar6 = (QArrayData *)thunk_FUN_140282190(local_res8);
  }
  local_108 = pQVar6;
  if (pQVar6 == (QArrayData *)0x0) {
    local_100 = (QArrayData *)0x0;
    pQVar7 = pQVar13;
  }
  else {
    pQVar7 = operator_new(0x18);
    if (pQVar7 + 0x10 != (QArrayData *)0x0) {
      *(QArrayData **)(pQVar7 + 0x10) = pQVar6;
    }
    if (pQVar7 != (QArrayData *)0x0) {
      *(undefined1 **)(pQVar7 + 8) = &LAB_14000c2d4;
      *(int *)(pQVar7 + 4) = 1;
      *(int *)pQVar7 = 1;
    }
    local_res8 = pQVar7;
    local_100 = pQVar7;
    thunk_FUN_14007c8c0(pQVar7,pQVar6);
    FUN_1400109b5(&local_108,pQVar6);
  }
  QDateTime::operator=((QDateTime *)(pQVar6 + 0x30),local_f8);
  bVar1 = QImage::isNull(local_f0);
  pQVar14 = pQVar7;
  if (bVar1) {
    if (local_d0 != (QDateTime)0x0) {
      *(int *)(pQVar6 + 0x38) = 2;
      goto LAB_14028f224;
    }
    *(int *)(pQVar6 + 0x38) = 0;
    if (pQVar7 != (QArrayData *)0x0) {
      LOCK();
      *(int *)pQVar7 = *(int *)pQVar7 + 1;
      UNLOCK();
      LOCK();
      *(int *)(pQVar7 + 4) = *(int *)(pQVar7 + 4) + 1;
      UNLOCK();
      pQVar14 = local_100;
    }
    local_c8 = pQVar6;
    local_c0 = pQVar7;
    uVar8 = QRect::QRect(local_78,0,0,0,0);
    ppQVar12 = &local_c8;
LAB_14028f217:
    thunk_FUN_14028fcd0(param_1,ppQVar12,local_f0,uVar8);
  }
  else {
    FVar2 = QImage::format(local_f0);
    if (FVar2 != 0xb) {
      qt_assert("image.format() == QImage::Format_RGB555",
                "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabber-video\\videoStreamSequencer.cpp"
                ,0x56);
    }
    bVar1 = QImage::isNull((QImage *)(param_1 + 0x68));
    if (bVar1) {
LAB_14028f1d8:
      *(int *)(pQVar6 + 0x38) = 0;
      local_res18 = &local_98;
      if (pQVar7 != (QArrayData *)0x0) {
        LOCK();
        *(int *)pQVar7 = *(int *)pQVar7 + 1;
        UNLOCK();
        LOCK();
        *(int *)(pQVar7 + 4) = *(int *)(pQVar7 + 4) + 1;
        UNLOCK();
        pQVar14 = local_100;
      }
      local_98 = pQVar6;
      local_90 = pQVar7;
      uVar8 = QImage::rect(local_f0);
      ppQVar12 = &local_98;
      goto LAB_14028f217;
    }
    iVar3 = QImage::width((QImage *)(param_1 + 0x68));
    iVar4 = QImage::width(local_f0);
    if (iVar3 != iVar4) goto LAB_14028f1d8;
    iVar3 = QImage::height((QImage *)(param_1 + 0x68));
    iVar4 = QImage::height(local_f0);
    if (iVar3 != iVar4) goto LAB_14028f1d8;
    thunk_FUN_14028f980(&local_res8,local_f0,param_1 + 0x68);
    this = QArrayData::data(local_res8);
    pQVar9 = (QRect *)thunk_FUN_14028f940(&local_res8);
    for (; this != pQVar9; this = this + 0x10) {
      iVar3 = QRect::height(this);
      iVar4 = QRect::width(this);
      uVar15 = (int)pQVar13 + iVar3 * iVar4 * 2 + 8;
      pQVar13 = (QArrayData *)(ulonglong)uVar15;
      param_2 = local_res10;
    }
    iVar3 = QImage::byteCount(local_f0);
    if ((float)(int)uVar15 / (float)(iVar3 + 8) <= _DAT_1404498f0) {
      *(int *)(pQVar6 + 0x38) = 1;
      pvVar10 = QArrayData::data(local_res8);
      pvVar11 = (void *)thunk_FUN_14028f940(&local_res8);
      for (; pvVar10 != pvVar11; pvVar10 = (void *)((longlong)pvVar10 + 0x10)) {
        pQVar13 = pQVar14;
        if (pQVar14 != (QArrayData *)0x0) {
          LOCK();
          *(int *)pQVar14 = *(int *)pQVar14 + 1;
          UNLOCK();
          LOCK();
          *(int *)(pQVar14 + 4) = *(int *)(pQVar14 + 4) + 1;
          UNLOCK();
          pQVar13 = local_100;
        }
        local_a8 = pQVar6;
        local_a0 = pQVar14;
        thunk_FUN_14028fcd0(param_1,&local_a8,local_f0,pvVar10);
        pQVar14 = pQVar13;
      }
    }
    else {
      *(int *)(pQVar6 + 0x38) = 0;
      local_res18 = &local_b8;
      if (pQVar7 != (QArrayData *)0x0) {
        LOCK();
        *(int *)pQVar7 = *(int *)pQVar7 + 1;
        UNLOCK();
        LOCK();
        *(int *)(pQVar7 + 4) = *(int *)(pQVar7 + 4) + 1;
        UNLOCK();
        pQVar14 = local_100;
      }
      local_b8 = pQVar6;
      local_b0 = pQVar7;
      uVar8 = QImage::rect(local_f0);
      thunk_FUN_14028fcd0(param_1,&local_b8,local_f0,uVar8);
    }
    pQVar13 = local_res8;
    if (*(int *)local_res8 == 0) {
LAB_14028f1b1:
      QArrayData::data(local_res8);
      QArrayData::data(pQVar13);
      QArrayData::deallocate(pQVar13,0x10,8);
    }
    else if (*(int *)local_res8 != -1) {
      LOCK();
      iVar3 = *(int *)local_res8;
      *(int *)local_res8 = *(int *)local_res8 + -1;
      UNLOCK();
      pQVar14 = local_100;
      if (iVar3 == 1) goto LAB_14028f1b1;
    }
  }
LAB_14028f224:
  QDateTime::operator=((QDateTime *)(param_1 + 0x38),local_f8);
  QImage::operator=((QImage *)(param_1 + 0x40),local_f0);
  *param_2 = pQVar6;
  param_2[1] = pQVar14;
  thunk_FUN_140286490(0);
LAB_14028f25d:
  QImage::~QImage(local_f0);
  QDateTime::~QDateTime(local_f8);
  return param_2;
}

// =============================================================================
// Ghidra: FUN_14028fff0 @ 0x14028fff0
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

QImage * FUN_14028fff0(uint *param_1,QImage *param_2,QString *param_3)

{
  uint uVar1;
  char cVar2;
  bool bVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  Format FVar7;
  undefined8 uVar8;
  QString *pQVar9;
  QByteArray *pQVar10;
  char *pcVar11;
  basic_ostream<char,std::char_traits<char>_> *pbVar12;
  basic_ostream<char,struct_std::char_traits<char>_> *pbVar13;
  undefined8 uVar14;
  QImage *pQVar15;
  uchar *puVar16;
  ulonglong uVar17;
  undefined1 auStackY_1d8 [32];
  undefined ***local_198 [2];
  undefined4 local_188;
  QByteArray local_180 [8];
  QImage local_178 [28];
  int iStack_15c;
  undefined *local_158;
  undefined **local_150;
  basic_ostream<char,std::char_traits<char>_> local_148 [96];
  undefined8 local_e8;
  undefined4 local_e0;
  basic_ios<char,std::char_traits<char>_> local_d0 [104];
  undefined8 local_68;
  ulonglong local_60 [3];
  ulonglong local_48;
  ulonglong local_40;
  
  local_68 = 0xfffffffffffffffe;
  local_40 = DAT_14050e9d0 ^ (ulonglong)auStackY_1d8;
  local_188 = 0;
  iVar5 = *(int *)(param_3 + 0x18);
  if (iVar5 == 0x10) {
    FVar7 = 0xb;
  }
  else if (iVar5 == 0x18) {
    FVar7 = 0xd;
  }
  else {
    if (iVar5 != 0x20) {
      uVar8 = thunk_FUN_14018d250(local_198);
      cVar2 = thunk_FUN_14018ceb0(uVar8,30000);
      thunk_FUN_14018bd10(local_198);
      if (cVar2 != '\0') {
        local_158 = &DAT_14034ae60;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_d0);
        local_188 = 2;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_158,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_150,false);
        *(undefined ***)((longlong)&local_158 + (longlong)*(int *)(local_158 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_15c + (longlong)*(int *)(local_158 + 4)) =
             *(int *)(local_158 + 4) + -0x88;
        thunk_FUN_140058f40(&local_150,2);
        pQVar9 = (QString *)QString::QString((QString *)local_198,param_3);
        pQVar10 = (QByteArray *)QString::toLocal8Bit(pQVar9);
        iVar5 = *(int *)(*(longlong *)(param_3 + 0x20) + 4);
        pcVar11 = QByteArray::constData(pQVar10);
        uVar4 = FUN_14028fec0(param_3);
        uVar8 = thunk_FUN_140042af0(&local_158,"unknown bpp in frame sequencer");
        pbVar12 = (basic_ostream<char,std::char_traits<char>_> *)
                  thunk_FUN_140042af0(uVar8,", bpp: ");
        pbVar13 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                            (pbVar12,*(uint *)(param_3 + 0x18));
        pbVar12 = (basic_ostream<char,std::char_traits<char>_> *)
                  thunk_FUN_140042af0(pbVar13,", width: ");
        pbVar13 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                            (pbVar12,*(uint *)(param_3 + 0x10));
        pbVar12 = (basic_ostream<char,std::char_traits<char>_> *)
                  thunk_FUN_140042af0(pbVar13,", height: ");
        pbVar13 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                            (pbVar12,*(uint *)(param_3 + 0x14));
        pbVar12 = (basic_ostream<char,std::char_traits<char>_> *)
                  thunk_FUN_140042af0(pbVar13,", dataSize: ");
        pbVar13 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar12,iVar5);
        pbVar12 = (basic_ostream<char,std::char_traits<char>_> *)
                  thunk_FUN_140042af0(pbVar13,", bytesPerLine: ");
        pbVar13 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar12,uVar4);
        pbVar12 = (basic_ostream<char,std::char_traits<char>_> *)
                  thunk_FUN_140042af0(pbVar13,", isMonitorId: ");
        pbVar13 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                            (pbVar12,(bool)param_3[8]);
        pbVar12 = (basic_ostream<char,std::char_traits<char>_> *)
                  thunk_FUN_140042af0(pbVar13,", maxArea: ");
        pbVar13 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                            (pbVar12,*(uint *)(param_3 + 0xc));
        uVar8 = thunk_FUN_140042af0(pbVar13,", monitorOrWindowId: ");
        thunk_FUN_140042af0(uVar8,pcVar11);
        QByteArray::~QByteArray(local_180);
        QString::~QString((QString *)local_198);
        uVar8 = thunk_FUN_14007ce40(&local_158,local_60);
        uVar14 = thunk_FUN_14018d250(local_198);
        thunk_FUN_14018c4e0(uVar14,30000,uVar8,
                            "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabber-video\\videoStreamSequencer.cpp"
                           );
        thunk_FUN_14018bd10(local_198);
        if (0xf < local_48) {
          uVar17 = local_60[0];
          if (0xfff < local_48 + 1) {
            if ((local_60[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar17 = *(ulonglong *)(local_60[0] - 8);
            if (local_60[0] <= uVar17) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_60[0] - uVar17 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_60[0] - uVar17) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_140002199(uVar17);
        }
        thunk_FUN_14005cb60(local_d0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_d0);
      }
      pQVar15 = (QImage *)QImage::QImage((QImage *)local_60);
      QImage::QImage(param_2,pQVar15);
      param_2[0x20] = (QImage)0x1;
      pQVar15 = (QImage *)local_60;
      goto LAB_140290793;
    }
    FVar7 = 4;
  }
  uVar4 = *(uint *)(param_3 + 0x14);
  uVar1 = *(uint *)(*(longlong *)(param_3 + 0x20) + 4);
  puVar16 = (uchar *)QByteArray::constData((QByteArray *)(param_3 + 0x20));
  QImage::QImage(local_178,puVar16,*(int *)(param_3 + 0x10),uVar4,uVar1 / uVar4,FVar7,
                 (_func_void_void_ptr *)0x0,(void *)0x0);
  bVar3 = QImage::isNull(local_178);
  if (((bVar3) || (iVar5 = QImage::width(local_178), iVar5 < 1)) ||
     (iVar5 = QImage::height(local_178), iVar5 < 1)) {
    uVar8 = thunk_FUN_14018d250(local_198);
    cVar2 = thunk_FUN_14018ceb0(uVar8,40000);
    thunk_FUN_14018bd10(local_198);
    if (cVar2 != '\0') {
      local_158 = &DAT_14034ae60;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_d0);
      local_188 = 4;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_158,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_150,false);
      *(undefined ***)((longlong)&local_158 + (longlong)*(int *)(local_158 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_15c + (longlong)*(int *)(local_158 + 4)) =
           *(int *)(local_158 + 4) + -0x88;
      local_198[0] = &local_150;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_150);
      local_150 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_e8 = 0;
      local_e0 = 4;
      pQVar9 = (QString *)QString::QString((QString *)local_180,param_3);
      pQVar10 = (QByteArray *)QString::toLocal8Bit(pQVar9);
      iVar5 = *(int *)(*(longlong *)(param_3 + 0x20) + 4);
      pcVar11 = QByteArray::constData(pQVar10);
      uVar4 = FUN_14028fec0(param_3);
      uVar8 = thunk_FUN_140042af0(&local_158,"corrupted image processing in frame sequencer");
      pbVar12 = (basic_ostream<char,std::char_traits<char>_> *)thunk_FUN_140042af0(uVar8,", bpp: ");
      pbVar13 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                          (pbVar12,*(uint *)(param_3 + 0x18));
      pbVar12 = (basic_ostream<char,std::char_traits<char>_> *)
                thunk_FUN_140042af0(pbVar13,", width: ");
      pbVar13 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                          (pbVar12,*(uint *)(param_3 + 0x10));
      pbVar12 = (basic_ostream<char,std::char_traits<char>_> *)
                thunk_FUN_140042af0(pbVar13,", height: ");
      pbVar13 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                          (pbVar12,*(uint *)(param_3 + 0x14));
      pbVar12 = (basic_ostream<char,std::char_traits<char>_> *)
                thunk_FUN_140042af0(pbVar13,", dataSize: ");
      pbVar13 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar12,iVar5);
      pbVar12 = (basic_ostream<char,std::char_traits<char>_> *)
                thunk_FUN_140042af0(pbVar13,", bytesPerLine: ");
      pbVar13 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar12,uVar4);
      pbVar12 = (basic_ostream<char,std::char_traits<char>_> *)
                thunk_FUN_140042af0(pbVar13,", isMonitorId: ");
      pbVar13 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                          (pbVar12,(bool)param_3[8]);
      pbVar12 = (basic_ostream<char,std::char_traits<char>_> *)
                thunk_FUN_140042af0(pbVar13,", maxArea: ");
      pbVar13 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                          (pbVar12,*(uint *)(param_3 + 0xc));
      uVar8 = thunk_FUN_140042af0(pbVar13,", monitorOrWindowId: ");
      thunk_FUN_140042af0(uVar8,pcVar11);
      QByteArray::~QByteArray((QByteArray *)local_198);
      QString::~QString((QString *)local_180);
      uVar8 = thunk_FUN_14007ce40(&local_158,local_60);
      uVar14 = thunk_FUN_14018d250(local_198);
      thunk_FUN_14018c4e0(uVar14,40000,uVar8,
                          "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabber-video\\videoStreamSequencer.cpp"
                         );
      thunk_FUN_14018bd10(local_198);
      if (0xf < local_48) {
        uVar17 = local_60[0];
        if (0xfff < local_48 + 1) {
          if ((local_60[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar17 = *(ulonglong *)(local_60[0] - 8);
          if (local_60[0] <= uVar17) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_60[0] - uVar17 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_60[0] - uVar17) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002199(uVar17);
      }
      *(undefined ***)((longlong)&local_158 + (longlong)*(int *)(local_158 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_15c + (longlong)*(int *)(local_158 + 4)) =
           *(int *)(local_158 + 4) + -0x88;
      local_150 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140065bc0(&local_150);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_150);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_148);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_d0);
    }
    pQVar15 = (QImage *)QImage::QImage((QImage *)local_60);
    QImage::QImage(param_2,pQVar15);
    param_2[0x20] = (QImage)0x0;
    QImage::~QImage((QImage *)local_60);
  }
  else {
    QRect::QRect((QRect *)local_198);
    pQVar15 = (QImage *)QImage::copy(local_178,(QRect *)local_60);
    QImage::operator=(local_178,pQVar15);
    QImage::~QImage((QImage *)local_60);
    iVar5 = QImage::height(local_178);
    iVar6 = QImage::width(local_178);
    uVar4 = iVar5 * iVar6;
    if ((*param_1 < uVar4) || (FVar7 = QImage::format(local_178), FVar7 != 0xb)) {
      if (*param_1 < uVar4) {
        sqrt((double)*param_1 / (double)uVar4);
      }
      pQVar15 = (QImage *)thunk_FUN_1402d7930(param_1 + 0x22,local_60,local_178,0xb);
      QImage::operator=(local_178,pQVar15);
      QImage::~QImage((QImage *)local_60);
    }
    QImage::QImage(param_2,local_178);
    param_2[0x20] = (QImage)0x1;
  }
  pQVar15 = local_178;
LAB_140290793:
  QImage::~QImage(pQVar15);
  return param_2;
}

// =============================================================================
// Ghidra: __dyn_tls_init @ 0x1402daed0
// =============================================================================
/* Library Function - Single Match
    __dyn_tls_init
   
   Library: Visual Studio 2015 Release */

void __dyn_tls_init(undefined8 param_1,int param_2)

{
  code *pcVar1;
  undefined8 *puVar2;
  
  if (param_2 == 2) {
    for (puVar2 = &DAT_14033ea20; puVar2 != (undefined8 *)&DAT_14033ec38; puVar2 = puVar2 + 1) {
      pcVar1 = (code *)*puVar2;
      if (pcVar1 != (code *)0x0) {
        _guard_check_icall(pcVar1);
        (*pcVar1)();
      }
    }
  }
  return;
}

// =============================================================================
// Ghidra: tls_callback_0 @ 0x140013525
// =============================================================================
void tls_callback_0(undefined8 param_1,int param_2)

{
  code *pcVar1;
  undefined8 *puVar2;
  
  if (param_2 == 2) {
    for (puVar2 = &DAT_14033ea20; puVar2 != (undefined8 *)&DAT_14033ec38; puVar2 = puVar2 + 1) {
      pcVar1 = (code *)*puVar2;
      if (pcVar1 != (code *)0x0) {
        _guard_check_icall(pcVar1);
        (*pcVar1)();
      }
    }
  }
  return;
}

// =============================================================================
// Ghidra: tls_callback_1 @ 0x1402daf48
// =============================================================================
void tls_callback_1(undefined8 param_1,int param_2)

{
  longlong lVar1;
  code *pcVar2;
  int *piVar3;
  int iVar4;
  int *piVar5;
  
  if ((param_2 == 3) || (param_2 == 0)) {
    lVar1 = *(longlong *)((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8);
    piVar3 = *(int **)(lVar1 + 0x130);
    while (piVar3 != (int *)0x0) {
      iVar4 = *piVar3 + -1;
      if (-1 < iVar4) {
        piVar5 = piVar3 + ((longlong)iVar4 + 2) * 2;
        do {
          pcVar2 = *(code **)piVar5;
          if (pcVar2 != (code *)0x0) {
            _guard_check_icall(pcVar2);
            (*pcVar2)();
          }
          piVar5 = piVar5 + -2;
          iVar4 = iVar4 + -1;
        } while (-1 < iVar4);
      }
      piVar5 = *(int **)(piVar3 + 2);
      if (piVar5 != (int *)0x0) {
        _free_base(piVar3);
      }
      *(int **)(lVar1 + 0x130) = piVar5;
      piVar3 = piVar5;
    }
  }
  return;
}
