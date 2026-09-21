/*
 * FULL CODE.cpp
 * tlshost.exe / Kickidler Grabber - pseudocod extras cu Ghidra MCP
 * Data analizei: 2026-09-21
 *
 * IMPORTANT:
 * - Acesta este pseudocodul C/C++ produs de decompilator, nu codul-sursa original.
 * - Nu este garantat compilabil. Tipurile undefined*, numele FUN_/DAT_/LAB_,
 *   conventiile de apel si prototipurile reflecta inferenta Ghidra.
 * - Sunt pastrate adresele functiilor pentru verificare in baza Ghidra.
 * - Fisierul reuneste traseele aplicatiei relevante: startup, serviciu Windows,
 *   proxy, sesiuni, audio, informatii si video. Binarul contine 29.461 functii;
 *   bibliotecile Qt/CRT/STL si instantiarile repetitive nu sunt reproduse integral.
 */


// =============================================================================
// STARTUP SI MODURI
// =============================================================================

// -----------------------------------------------------------------------------
// Ghidra @ 0x14000e83b
// -----------------------------------------------------------------------------
void entry(void)

{
  __security_init_cookie();
  FUN_14029d744();
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14029d744
// -----------------------------------------------------------------------------
int FUN_14029d744(void)

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
  if (DAT_140548040 == 1) {
                    /* WARNING: Subroutine does not return */
    __scrt_fastfail(7);
  }
  if (DAT_140548040 == 0) {
    DAT_140548040 = 1;
    iVar6 = _initterm_e(&DAT_140345d48,&DAT_140346188);
    if (iVar6 != 0) {
      return 0xff;
    }
    _initterm(&DAT_140344000,&DAT_140345908);
    DAT_140548040 = 2;
  }
  else {
    bVar2 = true;
  }
  __scrt_release_startup_lock(uVar4);
  plVar7 = (longlong *)thunk_FUN_14029e828();
  if ((*plVar7 != 0) && (cVar3 = __scrt_is_nonwritable_in_current_image(plVar7), cVar3 != '\0')) {
    pcVar1 = (code *)*plVar7;
    _guard_check_icall(pcVar1);
    (*pcVar1)(0,2);
  }
  plVar7 = (longlong *)thunk_FUN_14029e834();
  if ((*plVar7 != 0) && (cVar3 = __scrt_is_nonwritable_in_current_image(plVar7), cVar3 != '\0')) {
    _register_thread_local_exe_atexit_callback(*plVar7);
  }
  uVar5 = __scrt_get_show_window_mode();
  uVar8 = _get_narrow_winmain_command_line();
  iVar6 = thunk_FUN_1402dfd00(&IMAGE_DOS_HEADER_140000000,0,uVar8,uVar5);
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

// -----------------------------------------------------------------------------
// Ghidra @ 0x140085840
// -----------------------------------------------------------------------------
int run_tlshost_application(int argc,undefined8 *argv)

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
  local_res8[0] = argc;
  local_res10 = QString::fromAscii_helper("KickidlerGrabber",0x10);
  QCoreApplication::setApplicationName((QString *)&local_res10);
  QString::~QString((QString *)&local_res10);
  local_res18 = (QTypedArrayData<unsigned_short> *)&local_res10;
  uVar5 = QFile::decodeName((char *)&local_res10);
  uVar6 = thunk_FUN_140076b10();
  thunk_FUN_14010e680(uVar6,1,uVar5);
  uVar5 = thunk_FUN_140085510(&local_res10,0);
  thunk_FUN_140187970(uVar5);
  QString::~QString((QString *)&local_res10);
  lVar11 = -1;
  if (local_res8[0] < 2) {
LAB_140085942:
    bVar1 = false;
  }
  else {
    local_res18 = QString::fromAscii_helper("-utilize-invite",0xf);
    bVar12 = true;
    local_res10 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_res10._4_4_,1);
    pcVar9 = (char *)argv[1];
    iVar3 = *(int *)(local_res18 + 4);
    pQVar7 = QString::constData((QString *)&local_res18);
    iVar3 = QString::compare_helper(pQVar7,iVar3,pcVar9,-1,1);
    if (iVar3 != 0) goto LAB_140085942;
    bVar1 = true;
  }
  if (bVar12) {
    QString::~QString((QString *)&local_res18);
  }
  bVar12 = false;
  if (bVar1) {
    thunk_FUN_1400963f0(local_120,1,local_res8,argv);
    iVar3 = QCoreApplication::exec();
    thunk_FUN_140096e90(local_120);
    return iVar3;
  }
  if (local_res8[0] == 3) {
    local_res18 = QString::fromAscii_helper("-encrypt",8);
    bVar12 = true;
    local_res10 = (QTypedArrayData<unsigned_short> *)
                  ((ulonglong)local_res10 & 0xffffffff00000000 | 2);
    pcVar9 = (char *)argv[1];
    iVar3 = *(int *)(local_res18 + 4);
    pQVar7 = QString::constData((QString *)&local_res18);
    iVar3 = QString::compare_helper(pQVar7,iVar3,pcVar9,-1,1);
    if (iVar3 != 0) goto LAB_1400859e9;
    bVar1 = true;
  }
  else {
LAB_1400859e9:
    bVar1 = false;
  }
  if (bVar12) {
    QString::~QString((QString *)&local_res18);
  }
  bVar12 = false;
  if (bVar1) {
    pcVar9 = (char *)argv[2];
    if (pcVar9 != (char *)0x0) {
      do {
        lVar11 = lVar11 + 1;
      } while (pcVar9[lVar11] != '\0');
    }
    local_res10 = QString::fromAscii_helper(pcVar9,(int)lVar11);
    pQVar8 = (QString *)thunk_FUN_140180a50(&local_res20,&local_res10);
    this = cout_exref;
    this_00 = (QByteArray *)QString::toLocal8Bit(pQVar8);
    pcVar9 = QByteArray::data(this_00);
    thunk_FUN_140042b60(this,pcVar9);
    QByteArray::~QByteArray((QByteArray *)&local_res18);
    std::basic_ostream<char,std::char_traits<char>_>::operator<<
              ((basic_ostream<char,std::char_traits<char>_> *)this,
               (_func_basic_ostream<char,struct_std::char_traits<char>_>_ptr_basic_ostream<char,struct_std::char_traits<char>_>_ptr
                *)&LAB_14000d882);
    QString::~QString((QString *)&local_res20);
    QString::~QString((QString *)&local_res10);
    return 0;
  }
  if (local_res8[0] < 4) {
LAB_140085ae2:
    bVar1 = false;
  }
  else {
    local_res18 = QString::fromAscii_helper("report",6);
    bVar12 = true;
    local_res10 = (QTypedArrayData<unsigned_short> *)
                  ((ulonglong)local_res10 & 0xffffffff00000000 | 4);
    cVar2 = thunk_FUN_140084790(argv[1],&local_res18);
    if (cVar2 == '\0') goto LAB_140085ae2;
    bVar1 = true;
  }
  if (bVar12) {
    QString::~QString((QString *)&local_res18);
  }
  if (!bVar1) {
    uVar5 = thunk_FUN_140085510(&local_res20,8);
    uVar5 = thunk_FUN_140061140(&local_res18,uVar5,&DAT_1403544d4);
    uVar5 = thunk_FUN_140061140(&local_res10,uVar5,"grabber");
    thunk_FUN_1401904b0(uVar5,0xa00000,10);
    QString::~QString((QString *)&local_res18);
    QString::~QString((QString *)&local_res20);
    if (0 < local_res8[0]) {
      pQVar8 = (QString *)QString::fromLocal8Bit((char *)&local_res20,(int)*argv);
      this_01 = (QFileInfo *)QFileInfo::QFileInfo((QFileInfo *)&local_res18,pQVar8);
      uVar5 = QFileInfo::fileName(this_01);
      thunk_FUN_140091840(uVar5);
      QFileInfo::~QFileInfo((QFileInfo *)&local_res18);
      QString::~QString((QString *)&local_res20);
    }
    thunk_FUN_14008c740(local_c8,local_res8[0],argv);
    iVar3 = thunk_FUN_140110690(local_b8);
    thunk_FUN_14008cc50(local_c8);
    return iVar3;
  }
  if (local_res8[0] < 5) {
    pcVar9 = "";
  }
  else {
    pcVar9 = (char *)argv[4];
    if (pcVar9 == (char *)0x0) {
      lVar10 = 0xffffffff;
      goto LAB_140085b29;
    }
  }
  lVar10 = -1;
  do {
    lVar10 = lVar10 + 1;
  } while (pcVar9[lVar10] != '\0');
LAB_140085b29:
  local_res20 = QString::fromAscii_helper(pcVar9,(int)lVar10);
  pcVar9 = (char *)argv[3];
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
  pcVar9 = (char *)argv[2];
  if (pcVar9 != (char *)0x0) {
    do {
      lVar11 = lVar11 + 1;
    } while (pcVar9[lVar11] != '\0');
  }
  local_res10 = QString::fromAscii_helper(pcVar9,(int)lVar11);
  uVar4 = thunk_FUN_14011f050(&local_res10);
  thunk_FUN_14011b0c0(local_108,1,uVar4,&local_res18,&local_res20,local_res8,argv);
  QString::~QString((QString *)&local_res10);
  QString::~QString((QString *)&local_res18);
  QString::~QString((QString *)&local_res20);
  iVar3 = QCoreApplication::exec();
  thunk_FUN_14011bbe0(local_108);
  return iVar3;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140089540
// -----------------------------------------------------------------------------
void ** build_component_profile(QListData *profileOutput,int executableName,int param_3)

{
  int iVar1;
  void **ppvVar2;
  Data *pDVar3;
  void **ppvVar4;
  void **ppvVar5;
  longlong lVar6;
  Data *this;
  int local_res10 [4];
  Data *local_res20;
  undefined8 uVar7;
  
  uVar7 = 0xfffffffffffffffe;
  local_res10[0] = executableName;
  ppvVar2 = QListData::begin(profileOutput);
  pDVar3 = QListData::detach_grow(profileOutput,local_res10,param_3);
  local_res20 = pDVar3;
  ppvVar4 = QListData::begin(profileOutput);
  lVar6 = (longlong)local_res10[0];
  ppvVar5 = QListData::begin(profileOutput);
  thunk_FUN_14008a640(profileOutput,ppvVar5,ppvVar4 + lVar6,ppvVar2,uVar7);
  lVar6 = (longlong)local_res10[0];
  ppvVar4 = QListData::end(profileOutput);
  ppvVar5 = QListData::begin(profileOutput);
  thunk_FUN_14008a640(profileOutput,ppvVar5 + (longlong)local_res10[0] + (longlong)param_3,ppvVar4,
                      ppvVar2 + lVar6);
  if (*(int *)pDVar3 != 0) {
    if (*(int *)pDVar3 == -1) goto LAB_14008963b;
    LOCK();
    iVar1 = *(int *)pDVar3;
    *(int *)pDVar3 = *(int *)pDVar3 + -1;
    UNLOCK();
    if (iVar1 != 1) goto LAB_14008963b;
  }
  this = pDVar3 + ((longlong)*(int *)(pDVar3 + 0xc) + 2) * 8;
  iVar1 = *(int *)(pDVar3 + 8);
  while (pDVar3 + ((longlong)iVar1 + 2) * 8 != this) {
    this = this + -8;
    QNetworkProxy::~QNetworkProxy((QNetworkProxy *)this);
  }
  QListData::dispose(pDVar3);
LAB_14008963b:
  ppvVar2 = QListData::begin(profileOutput);
  return ppvVar2 + local_res10[0];
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140089af0
// -----------------------------------------------------------------------------
undefined8 * build_component_profile(undefined8 *param_1,QString *param_2)

{
  int iVar1;
  QString *pQVar2;
  QTypedArrayData<unsigned_short> *pQVar3;
  QString local_res18 [8];
  
  pQVar2 = (QString *)QString::fromLatin1((char *)local_res18,0x40354c88);
  iVar1 = QString::compare(param_2,pQVar2,0);
  QString::~QString(local_res18);
  if (iVar1 != 0) {
    pQVar2 = (QString *)QString::fromLatin1((char *)local_res18,0x40354c98);
    iVar1 = QString::compare(param_2,pQVar2,0);
    QString::~QString(local_res18);
    if (iVar1 != 0) {
      pQVar2 = (QString *)QString::fromLatin1((char *)local_res18,0x40354ca8);
      iVar1 = QString::compare(param_2,pQVar2,0);
      QString::~QString(local_res18);
      if (iVar1 != 0) {
        pQVar2 = (QString *)QString::fromLatin1((char *)local_res18,0x40354d30);
        iVar1 = QString::compare(param_2,pQVar2,0);
        QString::~QString(local_res18);
        if (iVar1 != 0) {
          pQVar2 = (QString *)QString::fromLatin1((char *)local_res18,0x40354d40);
          iVar1 = QString::compare(param_2,pQVar2,0);
          QString::~QString(local_res18);
          if (iVar1 != 0) {
            pQVar2 = (QString *)QString::fromLatin1((char *)local_res18,0x40354d50);
            iVar1 = QString::compare(param_2,pQVar2,0);
            QString::~QString(local_res18);
            if (iVar1 != 0) {
              pQVar2 = (QString *)QString::fromLatin1((char *)local_res18,0x40354d98);
              iVar1 = QString::compare(param_2,pQVar2,0);
              QString::~QString(local_res18);
              if (iVar1 != 0) {
                pQVar2 = (QString *)QString::fromLatin1((char *)local_res18,0x40354da8);
                iVar1 = QString::compare(param_2,pQVar2,0);
                QString::~QString(local_res18);
                if (iVar1 != 0) {
                  pQVar2 = (QString *)QString::fromLatin1((char *)local_res18,0x40354dc0);
                  iVar1 = QString::compare(param_2,pQVar2,0);
                  QString::~QString(local_res18);
                  if (iVar1 != 0) {
                    pQVar2 = (QString *)QString::fromLatin1((char *)local_res18,0x40354e30);
                    iVar1 = QString::compare(param_2,pQVar2,0);
                    QString::~QString(local_res18);
                    if (iVar1 != 0) {
                      pQVar2 = (QString *)QString::fromLatin1((char *)local_res18,0x40354e40);
                      iVar1 = QString::compare(param_2,pQVar2,0);
                      QString::~QString(local_res18);
                      if (iVar1 != 0) {
                        pQVar2 = (QString *)QString::fromLatin1((char *)local_res18,0x40354e58);
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
                          goto LAB_14008a01c;
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
                    goto LAB_14008a01c;
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
              goto LAB_14008a01c;
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
        goto LAB_14008a01c;
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
LAB_14008a01c:
  QString::~QString(param_2);
  return param_1;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140087110
// -----------------------------------------------------------------------------
/* WARNING: Function: _alloca_probe replaced with injection: alloca_probe */
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_140087110(undefined8 param_1)

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
  
  local_30 = 0x14008712c;
  local_1840 = 0xfffffffffffffffe;
  local_38 = DAT_1405169d0 ^ (ulonglong)auStackY_1b38;
  uVar21 = 0;
  local_1ad0 = (Node *)shared_null_exref;
  local_19b0 = param_1;
  iVar5 = WTSEnumerateSessionsW(0,0);
  if (iVar5 == 0) {
    uVar11 = thunk_FUN_14018fec0(local_1938);
    cVar4 = thunk_FUN_14018fb20(uVar11,40000);
    thunk_FUN_14018e980(local_1938);
    if (cVar4 != '\0') {
      local_1818 = &DAT_140351f40;
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
                thunk_FUN_140042b60(&local_1818,"WTSEnumerateSessionsW failed with error ");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar12,DVar6);
      uVar11 = thunk_FUN_14007d360(&local_1818,local_1118);
      uVar9 = thunk_FUN_14018fec0(local_1928);
      thunk_FUN_14018f150(uVar9,40000,uVar11);
      thunk_FUN_14018e980(local_1928);
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
        FUN_14000219e(uVar21);
      }
      *(undefined ***)((longlong)&local_1818 + (longlong)*(int *)(local_1818 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_181c + (longlong)*(int *)(local_1818 + 4)) =
           *(int *)(local_1818 + 4) + -0x88;
      local_1810 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140065d80(&local_1810);
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
            uVar11 = thunk_FUN_14018fec0(local_1830);
            cVar4 = thunk_FUN_14018fb20(uVar11,30000);
            thunk_FUN_14018e980(local_1830);
            if (cVar4 != '\0') {
              local_1548 = &DAT_140351f40;
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
                        thunk_FUN_140042b60(&local_1548,"WTSQuerySessionInformationW: ");
              std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar12,DVar6);
              uVar11 = thunk_FUN_14007d360(&local_1548,local_1078);
              uVar9 = thunk_FUN_14018fec0(local_1948);
              thunk_FUN_14018f150(uVar9,30000,uVar11);
              thunk_FUN_14018e980(local_1948);
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
                FUN_14000219e(uVar20);
              }
              thunk_FUN_14005cd10(local_14c0);
              std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                        (local_14c0);
            }
          }
          else {
            uVar24 = *local_1a00;
            WTSFreeMemory();
            if (((uVar24 & 0xfffffffa) == 0) && (uVar24 != 5)) {
              if (1 < *(uint *)(local_1ad0 + 0x10)) {
                thunk_FUN_140089420(&local_1ad0);
              }
              uVar24 = *(uint *)(local_1ad0 + 0x24) ^ uVar26;
              local_1a80 = uVar24;
              plVar8 = (longlong *)thunk_FUN_140089910(&local_1ad0,&local_1ab8,uVar24);
              if ((QHashData *)*plVar8 == (QHashData *)local_1ad0) {
                if (*(int *)(local_1ad0 + 0x20) <= *(int *)(local_1ad0 + 0x14)) {
                  QHashData::rehash((QHashData *)local_1ad0,*(short *)(local_1ad0 + 0x1e) + 1);
                  uVar24 = *(uint *)(local_1ad0 + 0x24) ^ uVar26;
                  local_1a80 = uVar24;
                  plVar8 = (longlong *)thunk_FUN_140089910(&local_1ad0,&local_1ab8,uVar24);
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
  uVar11 = build_component_profile(local_1970,uVar11);
  thunk_FUN_14008ba10(uVar11,local_1a88);
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
  uVar11 = thunk_FUN_1400611d0(local_19c8,&DAT_140354f84,local_1a88);
  thunk_FUN_140061140(&local_1aa0,uVar11);
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
    thunk_FUN_140089420(&local_1a78);
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
        uVar11 = thunk_FUN_14018fec0(local_1918);
        cVar4 = thunk_FUN_14018fb20(uVar11,30000);
        thunk_FUN_14018e980(local_1918);
        if (cVar4 != '\0') {
          local_1728 = &DAT_140351f40;
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
                    thunk_FUN_140042b60(&local_1728,"Failed to WTSQueryUserToken for sesssion ");
          pbVar13 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar12,uVar2);
          pbVar12 = (basic_ostream<char,std::char_traits<char>_> *)
                    thunk_FUN_140042b60(pbVar13,&DAT_14034df84);
          std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar12,DVar6);
          thunk_FUN_14007d360(&local_1728,local_10f8);
          uVar11 = thunk_FUN_14018fec0(local_1908);
          thunk_FUN_14018f150(uVar11,30000);
          thunk_FUN_14018e980(local_1908);
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
            FUN_14000219e(uVar21);
          }
          *(undefined ***)((longlong)&local_1728 + (longlong)*(int *)(local_1728 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_172c + (longlong)*(int *)(local_1728 + 4)) =
               *(int *)(local_1728 + 4) + -0x88;
          local_1720 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                       vftable;
          thunk_FUN_140065d80(&local_1720);
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
          uVar11 = thunk_FUN_14018fec0(local_18f8);
          cVar4 = thunk_FUN_14018fb20(uVar11,40000);
          thunk_FUN_14018e980(local_18f8);
          if (cVar4 != '\0') {
            local_1638 = &DAT_140351f40;
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
                      thunk_FUN_140042b60(&local_1638,"CreatePipe failed");
            std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar12,DVar6);
            thunk_FUN_14007d360(&local_1638,local_10d8);
            uVar11 = thunk_FUN_14018fec0(local_18e8);
            thunk_FUN_14018f150(uVar11,40000);
            thunk_FUN_14018e980(local_18e8);
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
              FUN_14000219e(uVar21);
            }
            *(undefined ***)((longlong)&local_1638 + (longlong)*(int *)(local_1638 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_163c + (longlong)*(int *)(local_1638 + 4)) =
                 *(int *)(local_1638 + 4) + -0x88;
            local_1630 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                         vftable;
            thunk_FUN_140065d80(&local_1630);
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
            uVar11 = thunk_FUN_14018fec0(local_18d8);
            cVar4 = thunk_FUN_14018fb20(uVar11,40000);
            thunk_FUN_14018e980(local_18d8);
            if (cVar4 != '\0') {
              local_1458 = &DAT_140351f40;
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
                        thunk_FUN_140042b60(&local_1458,"SetHandleInformation failed");
              std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar12,DVar6);
              thunk_FUN_14007d360(&local_1458,local_10b8);
              uVar11 = thunk_FUN_14018fec0(local_18c8);
              thunk_FUN_14018f150(uVar11,40000);
              thunk_FUN_14018e980(local_18c8);
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
                FUN_14000219e(uVar21);
              }
              thunk_FUN_14005cd10(local_13d0);
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
              uVar11 = thunk_FUN_14018fec0(local_18b8);
              cVar4 = thunk_FUN_14018fb20(uVar11,40000);
              thunk_FUN_14018e980(local_18b8);
              if (cVar4 != '\0') {
                local_12f8 = &DAT_140351f40;
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
                thunk_FUN_1400590e0(local_12f0,2);
                DVar6 = GetLastError();
                pbVar12 = (basic_ostream<char,std::char_traits<char>_> *)
                          thunk_FUN_140042b60(&local_12f8,"Failed to CreateProcessAsUser: ");
                pbVar13 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                                    (pbVar12,DVar6);
                uVar11 = thunk_FUN_140042b60(pbVar13,", hNewToken: ");
                this_01 = (QByteArray *)QString::toLocal8Bit(local_1a88);
                pcVar15 = QByteArray::data(this_01);
                thunk_FUN_140042b60(uVar11,pcVar15);
                QByteArray::~QByteArray(local_19c0);
                thunk_FUN_14007d360(&local_12f8,local_1098);
                uVar11 = thunk_FUN_14018fec0(local_18a8);
                thunk_FUN_14018f150(uVar11,40000);
                thunk_FUN_14018e980(local_18a8);
                if (0xf < local_1080) {
                  thunk_FUN_140065850(local_1098[0],local_1080 + 1);
                }
                thunk_FUN_14005cd10(local_1270);
                this_03 = local_1270;
LAB_1400885c8:
                std::basic_ios<char,std::char_traits<char>_>::
                ~basic_ios<char,std::char_traits<char>_>(this_03);
              }
            }
            else {
              WaitForSingleObject(local_1a20.hProcess,5000);
              BVar7 = ReadFile(local_1ab0,local_1038,0x1000,&local_1a7c,(LPOVERLAPPED)0x0);
              if (BVar7 == 0) {
                uVar11 = thunk_FUN_14018fec0(local_1868);
                cVar4 = thunk_FUN_14018fb20(uVar11,40000);
                thunk_FUN_14018e980(local_1868);
                if (cVar4 != '\0') {
                  local_1208 = &DAT_140351f40;
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
                  thunk_FUN_1400590e0(local_1200,2);
                  DVar6 = GetLastError();
                  pbVar12 = (basic_ostream<char,std::char_traits<char>_> *)
                            thunk_FUN_140042b60(&local_1208,"failed to ReadFile from pipe: ");
                  std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar12,DVar6);
                  thunk_FUN_14007d360(&local_1208,local_1058);
                  uVar11 = thunk_FUN_14018fec0(local_1858);
                  thunk_FUN_14018f150(uVar11,40000);
                  thunk_FUN_14018e980(local_1858);
                  thunk_FUN_14005cdb0(local_1058);
                  thunk_FUN_14005cd10(local_1180);
                  this_03 = local_1180;
                  goto LAB_1400885c8;
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
                  pcVar15 = &DAT_140354e9d + lVar16;
                  lVar16 = lVar22;
                } while (*pcVar15 != '\0');
                local_1a58 = QString::fromAscii_helper("uid",(int)lVar22);
                if (1 < *(uint *)local_1ac8) {
                  thunk_FUN_1400894b0(&local_1ac8);
                }
                lVar16 = thunk_FUN_140089a30(local_1ac8,&local_1a58);
                if (lVar16 == 0) {
                  QVariant::QVariant(local_1898);
                  plVar8 = (longlong *)thunk_FUN_14008a2b0(&local_1ac8,local_1838,&local_1a58);
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
                  thunk_FUN_1400894b0(&local_1ac8);
                }
                lVar16 = thunk_FUN_140089a30(local_1ac8,&local_1a50);
                if (lVar16 == 0) {
                  QVariant::QVariant(local_1888);
                  plVar8 = (longlong *)thunk_FUN_14008a2b0(&local_1ac8,local_1848);
                  lVar16 = *plVar8;
                  QVariant::~QVariant(local_1888);
                }
                QVariant::operator=((QVariant *)(lVar16 + 0x20),pQVar17);
                QVariant::~QVariant(local_1878);
                QJsonDocument::~QJsonDocument((QJsonDocument *)&local_1ac0);
                QString::~QString((QString *)&local_1a50);
                QByteArray::~QByteArray(local_1a28);
                QVariant::QVariant(local_19a8,(QMap<QString,QVariant> *)&local_1ac8);
                thunk_FUN_140086fa0(&local_1a98,local_19a8);
                QVariant::~QVariant(local_19a8);
                if (*(uint *)local_1ac8 == 0) {
LAB_140088471:
                  thunk_FUN_1400892b0(local_1ac8);
                }
                else if (*(uint *)local_1ac8 != 0xffffffff) {
                  LOCK();
                  uVar26 = *(uint *)local_1ac8;
                  *(uint *)local_1ac8 = *(uint *)local_1ac8 - 1;
                  UNLOCK();
                  pNVar25 = local_1a78;
                  pNVar18 = local_1a70;
                  if (uVar26 == 1) goto LAB_140088471;
                }
              }
            }
            CloseHandle(local_1a90);
            CloseHandle(local_1ab0);
            CloseHandle(local_1a20.hProcess);
            CloseHandle(local_1a20.hThread);
            CloseHandle(local_1aa8);
            thunk_FUN_14000219e(lpCommandLine);
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
LAB_1400886af:
    QHashData::free_helper((QHashData *)pNVar25,(_func_void_Node_ptr *)&LAB_140017b84);
  }
  else if (*(int *)(pNVar25 + 0x10) != -1) {
    LOCK();
    pNVar25 = pNVar25 + 0x10;
    iVar5 = *(int *)pNVar25;
    *(int *)pNVar25 = *(int *)pNVar25 + -1;
    UNLOCK();
    pNVar25 = local_1a78;
    if (iVar5 == 1) goto LAB_1400886af;
  }
  uVar11 = local_19b0;
  FUN_14008a880(local_19b0,&local_1a98);
  QString::~QString((QString *)&local_1aa0);
  QString::~QString(local_1a48);
  if (*(int *)local_1a98 != 0) {
    if (*(int *)local_1a98 == -1) goto LAB_140088718;
    LOCK();
    iVar5 = *(int *)local_1a98;
    *(int *)local_1a98 = *(int *)local_1a98 + -1;
    UNLOCK();
    if (iVar5 != 1) goto LAB_140088718;
  }
  thunk_FUN_140089070(&local_1a98,local_1a98);
LAB_140088718:
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
  QHashData::free_helper((QHashData *)local_1ad0,(_func_void_Node_ptr *)&LAB_140017b84);
  return uVar11;
}

// =============================================================================
// SERVICIU WINDOWS SI CICLU DE VIATA
// =============================================================================

// -----------------------------------------------------------------------------
// Ghidra @ 0x140076d90
// -----------------------------------------------------------------------------
QObject * FUN_140076d90(QObject *param_1,char *param_2)

{
  char cVar1;
  char cVar2;
  int iVar3;
  char *pcVar4;
  QObject *pQVar5;
  
  if (param_2 == (char *)0x0) {
    return (QObject *)0x0;
  }
  pcVar4 = param_2;
  do {
    cVar1 = *pcVar4;
    cVar2 = pcVar4[(longlong)"grabber::Service" - (longlong)param_2];
    if (cVar1 != cVar2) break;
    pcVar4 = pcVar4 + 1;
  } while (cVar2 != '\0');
  if (cVar1 == cVar2) {
    return param_1;
  }
  iVar3 = strcmp(param_2,"QtService<QGuiApplication>");
  if (iVar3 == 0) {
    pQVar5 = param_1 + 0x10;
    if (param_1 == (QObject *)0x0) {
      pQVar5 = (QObject *)0x0;
    }
    return pQVar5;
  }
  iVar3 = strcmp(param_2,"QAbstractNativeEventFilter");
  if (iVar3 == 0) {
    pQVar5 = param_1 + 0x28;
    if (param_1 == (QObject *)0x0) {
      pQVar5 = (QObject *)0x0;
    }
    return pQVar5;
  }
                    /* WARNING: Could not recover jumptable at 0x000140076e4b. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  pQVar5 = QObject::qt_metacast(param_1,param_2);
  return pQVar5;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14008ee70
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void configure_termservice_dependency(undefined8 param_1,int param_2)

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
  local_18 = DAT_1405169d0 ^ (ulonglong)auStackY_c8;
  FUN_14008f330();
  if (param_2 == 1) {
    hSCManager = OpenSCManagerW((LPCWSTR)0x0,(LPCWSTR)0x0,4);
    if (hSCManager == (SC_HANDLE)0x0) {
      DVar1 = GetLastError();
      pbVar3 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_140042b60(cerr_exref,"OpenSCManagerW failed: ");
      pbVar4 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar3,DVar1);
      std::basic_ostream<char,std::char_traits<char>_>::operator<<
                ((basic_ostream<char,std::char_traits<char>_> *)pbVar4,
                 (_func_basic_ostream<char,struct_std::char_traits<char>_>_ptr_basic_ostream<char,struct_std::char_traits<char>_>_ptr
                  *)&LAB_14000d882);
    }
    else {
      lpServiceName = (LPCWSTR)QString::toStdWString(local_68);
      if (7 < *(ulonglong *)(lpServiceName + 0xc)) {
        lpServiceName = *(LPCWSTR *)lpServiceName;
      }
      hService = OpenServiceW(hSCManager,lpServiceName,3);
      if (7 < local_40) {
        thunk_FUN_14008e880();
      }
      local_40 = 7;
      local_48 = 0;
      local_58[0] = 0;
      if (hService == (SC_HANDLE)0x0) {
        uVar5 = QString::toStdString(local_68);
        DVar1 = GetLastError();
        uVar6 = thunk_FUN_140042b60(cerr_exref,"OpenServiceW (");
        uVar5 = thunk_FUN_14008bad0(uVar6,uVar5);
        pbVar3 = (basic_ostream<char,std::char_traits<char>_> *)
                 thunk_FUN_140042b60(uVar5,") failed: ");
        pbVar4 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar3,DVar1);
        std::basic_ostream<char,std::char_traits<char>_>::operator<<
                  ((basic_ostream<char,std::char_traits<char>_> *)pbVar4,
                   (_func_basic_ostream<char,struct_std::char_traits<char>_>_ptr_basic_ostream<char,struct_std::char_traits<char>_>_ptr
                    *)&LAB_14000d882);
        thunk_FUN_14005cdb0(local_58);
      }
      else {
        BVar2 = ChangeServiceConfigW
                          (hService,0xffffffff,0xffffffff,0xffffffff,(LPCWSTR)0x0,(LPCWSTR)0x0,
                           (LPDWORD)0x0,L"TermService",(LPCWSTR)0x0,(LPCWSTR)0x0,(LPCWSTR)0x0);
        if (BVar2 == 0) {
          uVar5 = QString::toStdString(local_68);
          DVar1 = GetLastError();
          uVar6 = thunk_FUN_140042b60(cerr_exref,"ChangeServiceConfigW (");
          uVar5 = thunk_FUN_14008bad0(uVar6,uVar5);
          pbVar3 = (basic_ostream<char,std::char_traits<char>_> *)
                   thunk_FUN_140042b60(uVar5,") failed: ");
          pbVar4 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar3,DVar1);
          std::basic_ostream<char,std::char_traits<char>_>::operator<<
                    ((basic_ostream<char,std::char_traits<char>_> *)pbVar4,
                     (_func_basic_ostream<char,struct_std::char_traits<char>_>_ptr_basic_ostream<char,struct_std::char_traits<char>_>_ptr
                      *)&LAB_14000d882);
          thunk_FUN_14005cdb0(local_38);
        }
        CloseServiceHandle(hService);
      }
      CloseServiceHandle(hSCManager);
    }
  }
  QString::~QString(local_68);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14008f680
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void restart_after_update(longlong param_1)

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
  local_28 = DAT_1405169d0 ^ (ulonglong)auStack_1c8;
  local_190 = 0;
  uVar5 = thunk_FUN_14018fec0(local_160);
  cVar4 = thunk_FUN_14018fb20(uVar5,20000);
  thunk_FUN_14018e980(local_160);
  if (cVar4 != '\0') {
    local_138 = &DAT_140351f40;
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
    uVar5 = thunk_FUN_140042b60(&local_138,"restart now (after update): ");
    this = (QByteArray *)QString::toLocal8Bit(pQVar6);
    pcVar7 = QByteArray::data(this);
    thunk_FUN_140042b60(uVar5,pcVar7);
    QByteArray::~QByteArray(local_178);
    QString::~QString(local_170);
    uVar5 = thunk_FUN_14007d360(&local_138,local_48);
    uVar8 = thunk_FUN_14018fec0(local_150);
    local_1a8 = 0x167;
    thunk_FUN_14018f150(uVar8,20000,uVar5,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-grabber\\service.cpp");
    thunk_FUN_14018e980(local_150);
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
      FUN_14000219e(uVar10);
    }
    *(undefined ***)((longlong)&local_138 + (longlong)*(int *)(local_138 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_13c + (longlong)*(int *)(local_138 + 4)) =
         *(int *)(local_138 + 4) + -0x88;
    local_130 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_140065d80(&local_130);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_130);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_128);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_b0)
    ;
  }
  if (DAT_140526fb0 != 0) {
    plVar9 = (longlong *)thunk_FUN_140076bb0();
    (**(code **)(*plVar9 + 0x60))(plVar9);
  }
  if (*(longlong *)(param_1 + 0x70) != 0) {
    thunk_FUN_1400e83a0();
  }
  local_188 = QString::fromAscii_helper("-restart",8);
  local_198 = (Data *)shared_null_exref;
  pQVar6 = (QString *)QCoreApplication::applicationFilePath();
  thunk_FUN_1400665a0(&local_198,&local_188);
  QProcess::startDetached(pQVar6,(QStringList *)&local_198);
  QString::~QString((QString *)&local_180);
  pDVar3 = local_198;
  if (*(int *)local_198 != 0) {
    if (*(int *)local_198 == -1) goto LAB_14008f9ac;
    LOCK();
    iVar2 = *(int *)local_198;
    *(int *)local_198 = *(int *)local_198 + -1;
    UNLOCK();
    if (iVar2 != 1) goto LAB_14008f9ac;
  }
  pQVar6 = (QString *)(local_198 + ((longlong)*(int *)(local_198 + 0xc) + 2) * 8);
  pQVar1 = (QString *)(local_198 + ((longlong)*(int *)(local_198 + 8) + 2) * 8);
  while (pQVar1 != pQVar6) {
    pQVar6 = pQVar6 + -8;
    QString::~QString(pQVar6);
  }
  QListData::dispose(pDVar3);
LAB_14008f9ac:
  QString::~QString((QString *)&local_188);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14008fad0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14008fad0(longlong param_1)

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
  local_28 = DAT_1405169d0 ^ (ulonglong)auStack_188;
  local_158 = 0;
  if (*(char *)(param_1 + 0x78) != '\0') {
    *(undefined1 *)(param_1 + 0x78) = 0;
    uVar2 = thunk_FUN_14018fec0(local_150);
    cVar1 = thunk_FUN_14018fb20(uVar2,20000);
    thunk_FUN_14018e980(local_150);
    if (cVar1 != '\0') {
      local_138 = &DAT_140351f40;
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
      thunk_FUN_140042b60(&local_138,"resume suspend");
      uVar2 = thunk_FUN_14007d360(&local_138,local_48);
      uVar3 = thunk_FUN_14018fec0(local_150);
      local_168 = 0x156;
      thunk_FUN_14018f150(uVar3,20000,uVar2,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-grabber\\service.cpp");
      thunk_FUN_14018e980(local_150);
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
        FUN_14000219e(uVar5);
      }
      *(undefined ***)((longlong)&local_138 + (longlong)*(int *)(local_138 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_140 + (longlong)*(int *)(local_138 + 4) + 4) =
           *(int *)(local_138 + 4) + -0x88;
      local_130 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140065d80(&local_130);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_130);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_128);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_b0);
    }
    plVar4 = (longlong *)thunk_FUN_140076bb0();
    (**(code **)(*plVar4 + 0x70))(plVar4);
    if (*(longlong *)(param_1 + 0x70) != 0) {
      thunk_FUN_1400e7be0();
    }
  }
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14008fdb0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14008fdb0(longlong param_1)

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
  local_28 = DAT_1405169d0 ^ (ulonglong)auStack_188;
  local_158 = 0;
  if (*(char *)(param_1 + 0x78) == '\0') {
    *(undefined1 *)(param_1 + 0x78) = 1;
    uVar2 = thunk_FUN_14018fec0(local_150);
    cVar1 = thunk_FUN_14018fb20(uVar2,20000);
    thunk_FUN_14018e980(local_150);
    if (cVar1 != '\0') {
      local_138 = &DAT_140351f40;
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
      thunk_FUN_140042b60(&local_138,"suspend");
      uVar2 = thunk_FUN_14007d360(&local_138,local_48);
      uVar3 = thunk_FUN_14018fec0(local_150);
      local_168 = 0x143;
      thunk_FUN_14018f150(uVar3,20000,uVar2,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-grabber\\service.cpp");
      thunk_FUN_14018e980(local_150);
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
        FUN_14000219e(uVar5);
      }
      *(undefined ***)((longlong)&local_138 + (longlong)*(int *)(local_138 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_140 + (longlong)*(int *)(local_138 + 4) + 4) =
           *(int *)(local_138 + 4) + -0x88;
      local_130 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140065d80(&local_130);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_130);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_128);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_b0);
    }
    plVar4 = (longlong *)thunk_FUN_140076bb0();
    (**(code **)(*plVar4 + 0x68))(plVar4);
    if (*(longlong *)(param_1 + 0x70) != 0) {
      thunk_FUN_1400e8440();
    }
  }
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140090090
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void perform_active_or_passive_work(longlong param_1)

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
  local_10 = DAT_1405169d0 ^ (ulonglong)auStack_178;
  local_138[0] = 0;
  uVar3 = thunk_FUN_140090630();
  cVar1 = thunk_FUN_1400da3b0(uVar3);
  if (cVar1 == '\0') {
    if (*(char *)(param_1 + 0x79) != '\0') {
      thunk_FUN_1400e83a0(*(undefined8 *)(param_1 + 0x70));
      uVar3 = thunk_FUN_140076a70();
      thunk_FUN_140103680(uVar3);
      *(undefined1 *)(param_1 + 0x79) = 0;
    }
    uVar3 = thunk_FUN_14018fec0(local_148);
    cVar1 = thunk_FUN_14018fb20(uVar3,20000);
    thunk_FUN_14018e980(local_148);
    if (cVar1 != '\0') {
      local_128 = &DAT_140351f40;
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
      thunk_FUN_140042b60(&local_128,"time synchronization required for useful work");
      uVar3 = thunk_FUN_14007d360(&local_128,local_30);
      uVar4 = thunk_FUN_14018fec0(local_148);
      local_158 = 0x18b;
      thunk_FUN_14018f150(uVar4,20000,uVar3,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-grabber\\service.cpp");
      thunk_FUN_14018e980(local_148);
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
        FUN_14000219e(uVar6);
      }
      *(undefined ***)((longlong)&local_128 + (longlong)*(int *)(local_128 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_12c + (longlong)*(int *)(local_128 + 4)) =
           *(int *)(local_128 + 4) + -0x88;
      local_120 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140065d80(&local_120);
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
    uVar3 = thunk_FUN_140076bb0();
    cVar1 = thunk_FUN_1400a6430(uVar3);
    if (cVar1 == '\0') {
      if (*(char *)(param_1 + 0x79) != '\0') {
        thunk_FUN_1400e83a0(*(undefined8 *)(param_1 + 0x70));
        uVar3 = thunk_FUN_140076a70();
        thunk_FUN_140103680(uVar3);
        *(undefined1 *)(param_1 + 0x79) = 0;
      }
      uVar3 = thunk_FUN_14018fec0(local_148);
      cVar1 = thunk_FUN_14018fb20(uVar3,20000);
      thunk_FUN_14018e980(local_148);
      if (cVar1 != '\0') {
        local_128 = &DAT_140351f40;
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
        thunk_FUN_1400590e0(&local_120,2);
        uVar3 = thunk_FUN_140076bb0();
        uVar2 = thunk_FUN_1400a9540(uVar3);
        this = (QString *)thunk_FUN_1400906d0(local_148,uVar2);
        uVar3 = thunk_FUN_140042b60(&local_128,"passive work: ");
        this_00 = (QByteArray *)QString::toLocal8Bit(this);
        pcVar5 = QByteArray::data(this_00);
        thunk_FUN_140042b60(uVar3,pcVar5);
        QByteArray::~QByteArray((QByteArray *)local_138);
        QString::~QString((QString *)local_148);
        uVar3 = thunk_FUN_14007d360(&local_128,local_30);
        uVar4 = thunk_FUN_14018fec0(local_148);
        local_158 = 0x191;
        thunk_FUN_14018f150(uVar4,20000,uVar3,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-grabber\\service.cpp")
        ;
        thunk_FUN_14018e980(local_148);
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
          FUN_14000219e(uVar6);
        }
        thunk_FUN_14005cd10(local_a0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_a0);
      }
    }
    else if (*(char *)(param_1 + 0x79) == '\0') {
      uVar3 = thunk_FUN_140076a70();
      uVar4 = thunk_FUN_140076bb0();
      thunk_FUN_140104740(uVar3,0,uVar4);
      thunk_FUN_1400e7fc0(*(undefined8 *)(param_1 + 0x70));
      *(undefined1 *)(param_1 + 0x79) = 1;
    }
  }
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140090890
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140090890(int param_1,undefined8 param_2,QString *param_3)

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
  local_10 = DAT_1405169d0 ^ (ulonglong)auStack_168;
  local_138[0] = 0;
  if (param_1 == 0) {
    uVar2 = thunk_FUN_14018fec0(local_138);
    cVar1 = thunk_FUN_14018fb20(uVar2,10000);
    thunk_FUN_14018e980(local_138);
    if (cVar1 == '\0') {
      return;
    }
    local_128 = &DAT_140351f40;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_a0);
    local_138[0] = 1;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
    *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_12c + (longlong)*(int *)(local_128 + 4)) =
         *(int *)(local_128 + 4) + -0x88;
    thunk_FUN_1400590e0(local_120,2);
    uVar2 = thunk_FUN_140042b60(&local_128,"qt say: [");
    pQVar3 = (QByteArray *)QString::toLocal8Bit(param_3);
    pcVar4 = QByteArray::data(pQVar3);
    thunk_FUN_140042b60(uVar2,pcVar4);
    QByteArray::~QByteArray((QByteArray *)local_138);
    thunk_FUN_140042b60(uVar2,&DAT_14034e278);
    uVar2 = thunk_FUN_14007d360(&local_128,local_30);
    uVar5 = thunk_FUN_14018fec0(local_138);
    local_148 = 0x74;
    thunk_FUN_14018f150(uVar5,10000,uVar2,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-grabber\\service.cpp");
    thunk_FUN_14018e980(local_138);
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
      FUN_14000219e(uVar6);
    }
  }
  else if (param_1 == 1) {
    uVar2 = thunk_FUN_14018fec0(local_138);
    cVar1 = thunk_FUN_14018fb20(uVar2,30000);
    thunk_FUN_14018e980(local_138);
    if (cVar1 == '\0') {
      return;
    }
    local_128 = &DAT_140351f40;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_a0);
    local_138[0] = 2;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
    *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_12c + (longlong)*(int *)(local_128 + 4)) =
         *(int *)(local_128 + 4) + -0x88;
    thunk_FUN_1400590e0(local_120,2);
    uVar2 = thunk_FUN_140042b60(&local_128,"qt say: [");
    pQVar3 = (QByteArray *)QString::toLocal8Bit(param_3);
    pcVar4 = QByteArray::data(pQVar3);
    thunk_FUN_140042b60(uVar2,pcVar4);
    QByteArray::~QByteArray((QByteArray *)local_138);
    thunk_FUN_140042b60(uVar2,&DAT_14034e278);
    uVar2 = thunk_FUN_14007d360(&local_128,local_30);
    uVar5 = thunk_FUN_14018fec0(local_138);
    local_148 = 0x77;
    thunk_FUN_14018f150(uVar5,30000,uVar2,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-grabber\\service.cpp");
    thunk_FUN_14018e980(local_138);
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
      FUN_14000219e(uVar6);
    }
  }
  else if (param_1 == 2) {
    uVar2 = thunk_FUN_14018fec0(local_138);
    cVar1 = thunk_FUN_14018fb20(uVar2,40000);
    thunk_FUN_14018e980(local_138);
    if (cVar1 == '\0') {
      return;
    }
    local_128 = &DAT_140351f40;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_a0);
    local_138[0] = 4;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
    *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_12c + (longlong)*(int *)(local_128 + 4)) =
         *(int *)(local_128 + 4) + -0x88;
    thunk_FUN_1400590e0(local_120,2);
    uVar2 = thunk_FUN_140042b60(&local_128,"qt say: [");
    pQVar3 = (QByteArray *)QString::toLocal8Bit(param_3);
    pcVar4 = QByteArray::data(pQVar3);
    thunk_FUN_140042b60(uVar2,pcVar4);
    QByteArray::~QByteArray((QByteArray *)local_138);
    thunk_FUN_140042b60(uVar2,&DAT_14034e278);
    uVar2 = thunk_FUN_14007d360(&local_128,local_30);
    uVar5 = thunk_FUN_14018fec0(local_138);
    local_148 = 0x7a;
    thunk_FUN_14018f150(uVar5,40000,uVar2,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-grabber\\service.cpp");
    thunk_FUN_14018e980(local_138);
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
      FUN_14000219e(uVar6);
    }
  }
  else if (param_1 == 3) {
    uVar2 = thunk_FUN_14018fec0(local_138);
    cVar1 = thunk_FUN_14018fb20(uVar2,50000);
    thunk_FUN_14018e980(local_138);
    if (cVar1 == '\0') {
      return;
    }
    local_128 = &DAT_140351f40;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_a0);
    local_138[0] = 8;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
    *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_12c + (longlong)*(int *)(local_128 + 4)) =
         *(int *)(local_128 + 4) + -0x88;
    thunk_FUN_1400590e0(local_120,2);
    uVar2 = thunk_FUN_140042b60(&local_128,"qt say: [");
    pQVar3 = (QByteArray *)QString::toLocal8Bit(param_3);
    pcVar4 = QByteArray::data(pQVar3);
    thunk_FUN_140042b60(uVar2,pcVar4);
    QByteArray::~QByteArray((QByteArray *)local_138);
    thunk_FUN_140042b60(uVar2,&DAT_14034e278);
    uVar2 = thunk_FUN_14007d360(&local_128,local_30);
    uVar5 = thunk_FUN_14018fec0(local_138);
    local_148 = 0x7d;
    thunk_FUN_14018f150(uVar5,50000,uVar2,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-grabber\\service.cpp");
    thunk_FUN_14018e980(local_138);
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
      FUN_14000219e(uVar6);
    }
  }
  else {
    uVar2 = thunk_FUN_14018fec0(local_138);
    cVar1 = thunk_FUN_14018fb20(uVar2,20000);
    thunk_FUN_14018e980(local_138);
    if (cVar1 == '\0') {
      return;
    }
    local_128 = &DAT_140351f40;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_a0);
    local_138[0] = 0x10;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
    *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_12c + (longlong)*(int *)(local_128 + 4)) =
         *(int *)(local_128 + 4) + -0x88;
    thunk_FUN_1400590e0(local_120,2);
    uVar2 = thunk_FUN_140042b60(&local_128,"qt say: [");
    pQVar3 = (QByteArray *)QString::toLocal8Bit(param_3);
    pcVar4 = QByteArray::data(pQVar3);
    thunk_FUN_140042b60(uVar2,pcVar4);
    QByteArray::~QByteArray((QByteArray *)local_138);
    thunk_FUN_140042b60(uVar2,&DAT_14034e278);
    uVar2 = thunk_FUN_14007d360(&local_128,local_30);
    uVar5 = thunk_FUN_14018fec0(local_138);
    local_148 = 0x80;
    thunk_FUN_14018f150(uVar5,20000,uVar2,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-grabber\\service.cpp");
    thunk_FUN_14018e980(local_138);
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
      FUN_14000219e(uVar6);
    }
  }
  thunk_FUN_14005cd10(local_a0);
  std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_a0);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140091a90
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void start_grabber_service(QObject *param_1)

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
  local_38 = DAT_1405169d0 ^ (ulonglong)auStackY_1e8;
  uVar7 = 0;
  uVar12 = 0;
  local_1a8[0] = 0;
  QMutexLocker::QMutexLocker(local_98,(QBasicMutex *)(param_1 + 0x50));
  uVar2 = thunk_FUN_14018fec0(&local_1b8);
  cVar1 = thunk_FUN_14018fb20(uVar2,20000);
  thunk_FUN_14018e980(&local_1b8);
  if (cVar1 != '\0') {
    local_188 = &DAT_140351f40;
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
    thunk_FUN_140042b60(&local_188,"start");
    uVar2 = thunk_FUN_14007d360(&local_188,&local_58);
    uVar3 = thunk_FUN_14018fec0(&local_1b8);
    thunk_FUN_14018f150(uVar3,20000,uVar2,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-grabber\\service.cpp");
    thunk_FUN_14018e980(&local_1b8);
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
      FUN_14000219e(pQVar11);
    }
    *(undefined ***)((longlong)&local_188 + (longlong)*(int *)(local_188 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_18c + (longlong)*(int *)(local_188 + 4)) =
         *(int *)(local_188 + 4) + -0x88;
    local_180 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_140065d80(&local_180);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_180);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_178);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
              (local_100);
  }
  plVar4 = (longlong *)thunk_FUN_140076bb0();
  (**(code **)(*plVar4 + 0x58))(plVar4);
  local_1b8 = (undefined ***)perform_active_or_passive_work;
  uStack_1b0 = 0;
  local_58 = (QObject *)thunk_FUN_1400aef70;
  uStack_50 = 0;
  uVar2 = thunk_FUN_140076bb0();
  thunk_FUN_14008c040(local_1a8,uVar2,&local_58,param_1);
  QMetaObject::Connection::~Connection((Connection *)local_1a8);
  local_1b8 = (undefined ***)FUN_14000527c;
  uStack_1b0 = 0;
  local_58 = (QObject *)thunk_FUN_1400af8a0;
  uStack_50 = 0;
  uVar2 = thunk_FUN_140076bb0();
  thunk_FUN_14008be60(local_1a8,uVar2,&local_58,param_1);
  QMetaObject::Connection::~Connection((Connection *)local_1a8);
  local_1b8 = operator_new(0x70);
  uVar2 = uVar7;
  if (local_1b8 != (undefined ***)0x0) {
    uVar2 = thunk_FUN_1400e2510(local_1b8);
  }
  *(undefined8 *)(param_1 + 0x70) = uVar2;
  uVar2 = thunk_FUN_140087110(&local_1b8);
  uVar3 = thunk_FUN_140076bb0();
  uVar3 = thunk_FUN_1400a5450(uVar3,&local_198);
  uVar5 = thunk_FUN_140076bb0();
  uVar5 = thunk_FUN_1400a53e0(uVar5,local_1a8);
  uVar6 = thunk_FUN_140076c50();
  thunk_FUN_14012ffd0(uVar6,uVar5,uVar3,uVar2);
  QString::~QString((QString *)local_1a8);
  QString::~QString((QString *)&local_198);
  thunk_FUN_140089160(CONCAT44(iStack_1ac,uStack_1b0));
  local_1b8 = operator_new(0x38);
  uVar2 = uVar7;
  if (local_1b8 != (undefined ***)0x0) {
    uVar2 = thunk_FUN_140156820(local_1b8);
  }
  *(undefined8 *)(param_1 + 0x60) = uVar2;
  uVar2 = thunk_FUN_14008ee20(&local_198);
  uVar2 = build_component_profile(local_90,uVar2);
  uVar3 = thunk_FUN_140085510(&local_1b8,9);
  uVar3 = thunk_FUN_140061140(&local_58,uVar3,&DAT_1403544d4);
  uVar2 = thunk_FUN_1400610d0(local_1a8,uVar3,uVar2);
  thunk_FUN_140076bb0();
  cVar1 = thunk_FUN_140159340(*(undefined8 *)(param_1 + 0x60),uVar2);
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
      uVar2 = thunk_FUN_140159d70(local_1b8,*(undefined8 *)(param_1 + 0x60));
    }
    *(undefined8 *)(param_1 + 0x58) = uVar2;
    local_1b8 = (undefined ***)restart_after_update;
    uStack_1b0 = 0;
    local_58 = (QObject *)restart_after_update;
    uStack_50 = 0;
    iStack_4c = iStack_1ac;
    thunk_FUN_14008bd70(&local_1b8,uVar2,thunk_FUN_14015f910,param_1);
    QMetaObject::Connection::~Connection((Connection *)&local_1b8);
    thunk_FUN_140076bb0();
    thunk_FUN_140076bb0();
    uVar2 = QCoreApplication::applicationFilePath();
    uVar3 = thunk_FUN_14009cd20(&local_198);
    uVar5 = thunk_FUN_14009cda0(&local_58);
    uVar3 = thunk_FUN_1400610d0(&local_1b8,uVar5,uVar3);
    thunk_FUN_14015a3b0(*(undefined8 *)(param_1 + 0x58),uVar3,uVar2);
    QString::~QString((QString *)&local_1b8);
    QString::~QString((QString *)&local_58);
    QString::~QString((QString *)&local_198);
    QString::~QString((QString *)local_1a8);
    thunk_FUN_14015c710(*(undefined8 *)(param_1 + 0x58));
    local_1b8 = (undefined ***)thunk_FUN_1400aee50;
    uStack_1b0 = 0;
    local_198 = 0x40013ac0;
    uStack_194 = 1;
    uStack_190 = 0;
    iStack_18c = iStack_1ac;
    local_58 = param_1;
    uVar2 = thunk_FUN_140076bb0();
    thunk_FUN_14008c130(&local_1b8,uVar2,&local_198,*(undefined8 *)(param_1 + 0x60));
    QMetaObject::Connection::~Connection((Connection *)&local_1b8);
    FUN_14008d0f0(&local_58);
    pppuVar8 = operator_new(0x1b8);
    uVar2 = uVar7;
    local_1b8 = pppuVar8;
    if (pppuVar8 != (undefined ***)0x0) {
      uVar2 = thunk_FUN_140076bb0();
      uVar2 = thunk_FUN_14017e310(pppuVar8,uVar2);
    }
    *(undefined8 *)(param_1 + 0x68) = uVar2;
    local_1b8 = operator_new(0x10);
    if (local_1b8 != (undefined ***)0x0) {
      uVar7 = thunk_FUN_140093430(local_1b8);
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
    uVar2 = thunk_FUN_140090630();
    uVar7 = thunk_FUN_140076bb0();
    thunk_FUN_1400da980(uVar2,uVar7);
    uVar2 = thunk_FUN_14018fec0(&local_1b8);
    cVar1 = thunk_FUN_14018fb20(uVar2,20000);
    thunk_FUN_14018e980(&local_1b8);
    if (cVar1 != '\0') {
      local_188 = &DAT_140351f40;
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
      thunk_FUN_140042b60(&local_188,"started");
      uVar2 = thunk_FUN_14007d360(&local_188,&local_58);
      uVar7 = thunk_FUN_14018fec0(&local_1b8);
      thunk_FUN_14018f150(uVar7,20000,uVar2,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-grabber\\service.cpp");
      thunk_FUN_14018e980(&local_1b8);
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
        FUN_14000219e(pQVar11);
      }
      *(undefined ***)((longlong)&local_188 + (longlong)*(int *)(local_188 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_18c + (longlong)*(int *)(local_188 + 4)) =
           *(int *)(local_188 + 4) + -0x88;
      local_180 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140065d80(&local_180);
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
  uVar2 = thunk_FUN_14018fec0(&local_1b8);
  cVar1 = thunk_FUN_14018fb20(uVar2,50000);
  thunk_FUN_14018e980(&local_1b8);
  if (cVar1 != '\0') {
    local_188 = &DAT_140351f40;
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
    thunk_FUN_140042b60(&local_188,"unable to setup aup collector");
    uVar2 = thunk_FUN_14007d360(&local_188,&local_58);
    uVar7 = thunk_FUN_14018fec0(&local_1b8);
    thunk_FUN_14018f150(uVar7,50000,uVar2,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-grabber\\service.cpp");
    thunk_FUN_14018e980(&local_1b8);
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
      FUN_14000219e(pQVar11);
    }
    *(undefined ***)((longlong)&local_188 + (longlong)*(int *)(local_188 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_18c + (longlong)*(int *)(local_188 + 4)) =
         *(int *)(local_188 + 4) + -0x88;
    local_180 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_140065d80(&local_180);
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

// -----------------------------------------------------------------------------
// Ghidra @ 0x140092820
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void stop_grabber_service(longlong param_1)

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
  local_30 = DAT_1405169d0 ^ (ulonglong)auStack_198;
  uVar8 = 0;
  local_168 = 0;
  uVar4 = thunk_FUN_14018fec0(local_160);
  cVar1 = thunk_FUN_14018fb20(uVar4);
  thunk_FUN_14018e980(local_160);
  if (cVar1 != '\0') {
    local_148 = &DAT_140351f40;
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
    thunk_FUN_140042b60(&local_148,&DAT_140355c54);
    uVar4 = thunk_FUN_14007d360(&local_148,local_50);
    uVar5 = thunk_FUN_14018fec0(local_160);
    local_178 = 0xfa;
    thunk_FUN_14018f150(uVar5,20000,uVar4,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-grabber\\service.cpp");
    thunk_FUN_14018e980(local_160);
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
      FUN_14000219e(uVar7);
    }
    *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
         *(int *)(local_148 + 4) + -0x88;
    local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_140065d80(&local_140);
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
    uVar4 = thunk_FUN_14018fec0(local_160);
    cVar1 = thunk_FUN_14018fb20(uVar4);
    thunk_FUN_14018e980(local_160);
    if (cVar1 != '\0') {
      local_148 = &DAT_140351f40;
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
      thunk_FUN_140042b60(&local_148,"await startInitialization to be completed");
      uVar4 = thunk_FUN_14007d360(&local_148,local_50);
      uVar5 = thunk_FUN_14018fec0(local_160);
      local_178 = 0x100;
      thunk_FUN_14018f150(uVar5,20000,uVar4,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-grabber\\service.cpp");
      thunk_FUN_14018e980(local_160);
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
        FUN_14000219e(uVar7);
      }
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140065d80(&local_140);
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
    uVar4 = thunk_FUN_14018fec0(local_160);
    cVar1 = thunk_FUN_14018fb20(uVar4,20000);
    thunk_FUN_14018e980(local_160);
    if (cVar1 != '\0') {
      local_148 = &DAT_140351f40;
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
      thunk_FUN_140042b60(&local_148,"startInitialization completed, do real stop now");
      uVar4 = thunk_FUN_14007d360(&local_148,local_50);
      uVar5 = thunk_FUN_14018fec0(local_160);
      local_178 = 0x10e;
      thunk_FUN_14018f150(uVar5,20000,uVar4,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-grabber\\service.cpp");
      thunk_FUN_14018e980(local_160);
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
        FUN_14000219e(uVar7);
      }
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140065d80(&local_140);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_138);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_c0);
    }
  }
  uVar4 = thunk_FUN_140090630();
  thunk_FUN_1400daae0(uVar4);
  if (*(longlong *)(param_1 + 0x60) != 0) {
    thunk_FUN_1400e83a0();
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
  plVar6 = (longlong *)thunk_FUN_140076bb0();
  (**(code **)(*plVar6 + 0x60))(plVar6);
  plVar6 = *(longlong **)(param_1 + 0x70);
  if (plVar6 != (longlong *)0x0) {
    (**(code **)(*plVar6 + 0x18))(plVar6,1);
    *(undefined8 *)(param_1 + 0x70) = 0;
  }
  uVar4 = thunk_FUN_14018fec0(local_160);
  cVar1 = thunk_FUN_14018fb20(uVar4,20000);
  thunk_FUN_14018e980(local_160);
  if (cVar1 != '\0') {
    local_148 = &DAT_140351f40;
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
    thunk_FUN_140042b60(&local_148,"stopped");
    uVar4 = thunk_FUN_14007d360(&local_148,local_50);
    uVar5 = thunk_FUN_14018fec0(local_160);
    local_178 = 0x137;
    thunk_FUN_14018f150(uVar5,20000,uVar4,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-grabber\\service.cpp");
    thunk_FUN_14018e980(local_160);
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
      FUN_14000219e(uVar7);
    }
    *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
         *(int *)(local_148 + 4) + -0x88;
    local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_140065d80(&local_140);
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
// AUDIO
// =============================================================================

// -----------------------------------------------------------------------------
// Ghidra @ 0x1401d5a40
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1401d5a40(longlong param_1)

{
  char cVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  ulonglong uVar4;
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
  local_28 = DAT_1405169d0 ^ (ulonglong)auStack_188;
  local_158 = 0;
  cVar1 = thunk_FUN_14012e290();
  if (cVar1 != '\0') {
    uVar2 = thunk_FUN_14018fec0(local_150);
    cVar1 = thunk_FUN_14018fb20(uVar2,20000);
    thunk_FUN_14018e980(local_150);
    if (cVar1 != '\0') {
      local_138 = &DAT_140351f40;
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
      thunk_FUN_140042b60(&local_138,"node connection for audio closed");
      uVar2 = thunk_FUN_14007d360(&local_138,local_48);
      uVar3 = thunk_FUN_14018fec0(local_150);
      local_168 = 0x5a;
      thunk_FUN_14018f150(uVar3,20000,uVar2,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-audio\\audioSink.cpp"
                         );
      thunk_FUN_14018e980(local_150);
      if (0xf < local_30) {
        uVar4 = local_48[0];
        if (0xfff < local_30 + 1) {
          if ((local_48[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar4 = *(ulonglong *)(local_48[0] - 8);
          if (local_48[0] <= uVar4) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_48[0] - uVar4 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_48[0] - uVar4) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_14000219e(uVar4);
      }
      *(undefined ***)((longlong)&local_138 + (longlong)*(int *)(local_138 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_140 + (longlong)*(int *)(local_138 + 4) + 4) =
           *(int *)(local_138 + 4) + -0x88;
      local_130 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140065d80(&local_130);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_130);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_128);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_b0);
    }
  }
  *(undefined1 *)(param_1 + 0x1b0) = 1;
  thunk_FUN_14012ed50(param_1);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1401d60d0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1401d60d0(longlong param_1,undefined8 *param_2)

{
  char cVar1;
  undefined8 uVar2;
  longlong lVar3;
  QString *this;
  QByteArray *this_00;
  char *pcVar4;
  undefined8 uVar5;
  ulonglong uVar6;
  undefined1 auStack_1a8 [32];
  undefined4 local_188;
  ulonglong local_178;
  undefined8 uStack_170;
  undefined ***local_168 [2];
  undefined8 local_158;
  undefined8 local_150;
  undefined *local_148;
  undefined **local_140;
  basic_ostream<char,std::char_traits<char>_> local_138 [96];
  undefined8 local_d8;
  undefined4 local_d0;
  basic_ios<char,std::char_traits<char>_> local_c0 [104];
  ulonglong local_58;
  undefined8 local_50;
  ulonglong local_40;
  ulonglong local_38;
  
  local_158 = 0xfffffffffffffffe;
  local_38 = DAT_1405169d0 ^ (ulonglong)auStack_1a8;
  local_178 = local_178 & 0xffffffff00000000;
  local_150 = param_2;
  uVar2 = (**(code **)(*(longlong *)*param_2 + 0x60))((longlong *)*param_2,&local_178);
  lVar3 = thunk_FUN_1401d4fd0(*(undefined8 *)(param_1 + 0x1a8),uVar2);
  QString::~QString((QString *)&local_178);
  if (lVar3 == 0) {
    uVar2 = thunk_FUN_14018fec0(local_168);
    cVar1 = thunk_FUN_14018fb20(uVar2,30000);
    thunk_FUN_14018e980(local_168);
    if (cVar1 != '\0') {
      local_148 = &DAT_140351f40;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_c0);
      local_178 = CONCAT44(local_178._4_4_,1);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_148,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_140,false);
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_150 + (longlong)*(int *)(local_148 + 4) + 4) =
           *(int *)(local_148 + 4) + -0x88;
      local_168[0] = &local_140;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_d8 = 0;
      local_d0 = 4;
      this = (QString *)(**(code **)(*(longlong *)*param_2 + 0x60))((longlong *)*param_2,local_168);
      uVar2 = thunk_FUN_140042b60(&local_148,"audio: session shutdown for unknown sequencer ");
      this_00 = (QByteArray *)QString::toLocal8Bit(this);
      pcVar4 = QByteArray::data(this_00);
      thunk_FUN_140042b60(uVar2,pcVar4);
      QByteArray::~QByteArray((QByteArray *)&local_178);
      QString::~QString((QString *)local_168);
      uVar2 = thunk_FUN_14007d360(&local_148,&local_58);
      uVar5 = thunk_FUN_14018fec0(local_168);
      local_188 = 0x38;
      thunk_FUN_14018f150(uVar5,30000,uVar2,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-audio\\audioSink.cpp"
                         );
      thunk_FUN_14018e980(local_168);
      if (0xf < local_40) {
        uVar6 = local_58;
        if (0xfff < local_40 + 1) {
          if ((local_58 & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar6 = *(ulonglong *)(local_58 - 8);
          if (local_58 <= uVar6) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_58 - uVar6 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_58 - uVar6) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_14000219e(uVar6);
      }
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_150 + (longlong)*(int *)(local_148 + 4) + 4) =
           *(int *)(local_148 + 4) + -0x88;
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140065d80(&local_140);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_138);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_c0);
    }
  }
  else {
    local_178 = 0;
    uStack_170 = 0;
    uVar2 = (**(code **)(*(longlong *)*param_2 + 0x60))((longlong *)*param_2,local_168);
    thunk_FUN_1401d8ef0(param_1 + 0x1a8,&local_58,uVar2,&local_178);
    QString::~QString((QString *)local_168);
    thunk_FUN_1401d4cc0(uStack_170);
    uVar2 = (**(code **)(*(longlong *)*param_2 + 0x60))((longlong *)*param_2,&local_178);
    thunk_FUN_1401d85a0(param_1 + 0x1a8,uVar2);
    QString::~QString((QString *)&local_178);
    thunk_FUN_1401d4cc0(local_50);
  }
  thunk_FUN_1400e5920(param_2[1]);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1401d64e0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1401d64e0(longlong param_1,undefined8 *param_2)

{
  char cVar1;
  undefined8 uVar2;
  longlong lVar3;
  QString *this;
  QByteArray *this_00;
  char *pcVar4;
  undefined8 uVar5;
  QObject *pQVar6;
  ExternalRefCountData *this_01;
  ulonglong uVar7;
  undefined1 auStack_208 [32];
  undefined4 local_1e8;
  undefined4 local_1d8;
  undefined ***local_1d0;
  ExternalRefCountData *local_1c8;
  undefined8 local_1c0;
  int *local_1b8;
  QObject *local_1b0;
  ExternalRefCountData *local_1a8;
  QString local_1a0 [8];
  QByteArray local_198 [8];
  QString local_190 [8];
  undefined8 local_188;
  undefined8 *local_180;
  undefined1 local_178 [8];
  undefined1 local_170 [16];
  undefined1 local_160 [20];
  int iStack_14c;
  undefined *local_148;
  undefined **local_140;
  basic_ostream<char,std::char_traits<char>_> local_138 [96];
  undefined8 local_d8;
  undefined4 local_d0;
  basic_ios<char,std::char_traits<char>_> local_c0 [104];
  ulonglong local_58 [3];
  ulonglong local_40;
  ulonglong local_38;
  
  local_188 = 0xfffffffffffffffe;
  local_38 = DAT_1405169d0 ^ (ulonglong)auStack_208;
  this_01 = (ExternalRefCountData *)0x0;
  local_1d8 = 0;
  local_180 = param_2;
  uVar2 = (**(code **)(*(longlong *)*param_2 + 0x60))((longlong *)*param_2,local_1a0);
  lVar3 = thunk_FUN_1401d4fd0(*(undefined8 *)(param_1 + 0x1a8),uVar2);
  QString::~QString(local_1a0);
  if (lVar3 == 0) {
    local_1c8 = operator_new(0x90);
    pQVar6 = (QObject *)this_01;
    if (local_1c8 != (ExternalRefCountData *)0x0) {
      local_1c0 = *param_2;
      local_1b8 = (int *)param_2[1];
      if (local_1b8 != (int *)0x0) {
        LOCK();
        *local_1b8 = *local_1b8 + 1;
        UNLOCK();
        LOCK();
        local_1b8[1] = local_1b8[1] + 1;
        UNLOCK();
      }
      pQVar6 = (QObject *)thunk_FUN_1401dd480(local_1c8,&local_1c0,param_1);
    }
    local_1b0 = pQVar6;
    if (pQVar6 == (QObject *)0x0) {
      local_1a8 = (ExternalRefCountData *)0x0;
    }
    else {
      this_01 = operator_new(0x18);
      if (this_01 + 0x10 != (ExternalRefCountData *)0x0) {
        *(QObject **)(this_01 + 0x10) = pQVar6;
      }
      if (this_01 != (ExternalRefCountData *)0x0) {
        *(undefined1 **)(this_01 + 8) = &LAB_140008067;
        *(undefined4 *)(this_01 + 4) = 1;
        *(undefined4 *)this_01 = 1;
      }
      local_1c8 = this_01;
      local_1a8 = this_01;
      QtSharedPointer::ExternalRefCountData::setQObjectShared(this_01,pQVar6,true);
      FUN_140009642(&local_1b0,pQVar6);
    }
    thunk_FUN_1401e4180(pQVar6);
    uVar2 = (**(code **)(*(longlong *)*param_2 + 0x60))((longlong *)*param_2,&local_1d0);
    thunk_FUN_1401d53d0(param_1 + 0x1a8,local_178,uVar2,&local_1b0);
    QString::~QString((QString *)&local_1d0);
    thunk_FUN_1401d4cc0(this_01);
  }
  else {
    uVar2 = thunk_FUN_14018fec0(local_170);
    cVar1 = thunk_FUN_14018fb20(uVar2,30000);
    thunk_FUN_14018e980(local_170);
    if (cVar1 != '\0') {
      local_148 = &DAT_140351f40;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_c0);
      local_1d8 = 1;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_148,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_140,false);
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      local_1d0 = &local_140;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_d8 = 0;
      local_d0 = 4;
      this = (QString *)(**(code **)(*(longlong *)*param_2 + 0x60))((longlong *)*param_2,local_190);
      uVar2 = thunk_FUN_140042b60(&local_148,"audio: duplicate session startup ignored for ");
      this_00 = (QByteArray *)QString::toLocal8Bit(this);
      pcVar4 = QByteArray::data(this_00);
      thunk_FUN_140042b60(uVar2,pcVar4);
      QByteArray::~QByteArray(local_198);
      QString::~QString(local_190);
      uVar2 = thunk_FUN_14007d360(&local_148,local_58);
      uVar5 = thunk_FUN_14018fec0(local_160);
      local_1e8 = 0x2b;
      thunk_FUN_14018f150(uVar5,30000,uVar2,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-audio\\audioSink.cpp"
                         );
      thunk_FUN_14018e980(local_160);
      if (0xf < local_40) {
        uVar7 = local_58[0];
        if (0xfff < local_40 + 1) {
          if ((local_58[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar7 = *(ulonglong *)(local_58[0] - 8);
          if (local_58[0] <= uVar7) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_58[0] - uVar7 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_58[0] - uVar7) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_14000219e(uVar7);
      }
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140065d80(&local_140);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_138);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_c0);
    }
  }
  thunk_FUN_1400e5920(param_2[1]);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1401d7890
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1401d7890(longlong param_1,longlong *param_2)

{
  bool bVar1;
  char cVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  longlong *plVar5;
  QString *this;
  QByteArray *this_00;
  char *pcVar6;
  ulonglong uVar7;
  undefined ***pppuVar8;
  uint uVar9;
  undefined1 auStack_1b8 [32];
  undefined4 local_198;
  undefined8 local_188;
  undefined8 local_180;
  undefined *local_178;
  undefined **local_170;
  basic_ostream<char,std::char_traits<char>_> local_168 [96];
  undefined8 local_108;
  undefined4 local_100;
  basic_ios<char,std::char_traits<char>_> local_f0 [104];
  undefined8 local_88;
  undefined ***local_80;
  undefined8 uStack_78;
  ulonglong local_68;
  ulonglong local_60;
  undefined8 local_58;
  ulonglong local_48;
  ulonglong local_40;
  
  local_88 = 0xfffffffffffffffe;
  local_40 = DAT_1405169d0 ^ (ulonglong)auStack_1b8;
  local_188 = (QTypedArrayData<unsigned_short> *)((ulonglong)local_188 & 0xffffffff00000000);
  uVar3 = thunk_FUN_1401d6990();
  local_188 = QString::fromAscii_helper("grabberAudio",0xc);
  cVar2 = thunk_FUN_1401ce5e0(uVar3,&local_188);
  QString::~QString((QString *)&local_188);
  uVar9 = 0;
  if (cVar2 != '\0') {
    uVar3 = thunk_FUN_14018fec0(&local_80);
    cVar2 = thunk_FUN_14018fb20(uVar3,10000);
    thunk_FUN_14018e980(&local_80);
    uVar9 = 0;
    if (cVar2 != '\0') {
      local_178 = &DAT_140351f40;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_f0);
      uVar9 = 4;
      local_188 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_188._4_4_,4);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_178,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_170,false);
      *(undefined ***)((longlong)&local_178 + (longlong)*(int *)(local_178 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_180 + (longlong)*(int *)(local_178 + 4) + 4) =
           *(int *)(local_178 + 4) + -0x88;
      local_80 = &local_170;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_170);
      local_170 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_108 = 0;
      local_100 = 4;
      uVar3 = thunk_FUN_140042b60(&local_178,"audio: ");
      uVar3 = thunk_FUN_140042b60(uVar3,"grabber::AudioSink::recv");
      thunk_FUN_140042b60(uVar3," START TRACK");
      uVar3 = thunk_FUN_14007d360(&local_178,&local_60);
      uVar4 = thunk_FUN_14018fec0(&local_80);
      local_198 = 0x72;
      thunk_FUN_14018f150(uVar4,10000,uVar3,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-audio\\audioSink.cpp"
                         );
      thunk_FUN_14018e980(&local_80);
      if (0xf < local_48) {
        uVar7 = local_60;
        if (0xfff < local_48 + 1) {
          if ((local_60 & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar7 = *(ulonglong *)(local_60 - 8);
          if (local_60 <= uVar7) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_60 - uVar7 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_60 - uVar7) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_14000219e(uVar7);
      }
      *(undefined ***)((longlong)&local_178 + (longlong)*(int *)(local_178 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_180 + (longlong)*(int *)(local_178 + 4) + 4) =
           *(int *)(local_178 + 4) + -0x88;
      local_170 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140065d80(&local_170);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_170);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_168);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_f0);
    }
  }
  if (*(longlong *)(*param_2 + 8) == *(longlong *)(param_1 + 0x1a0)) {
    local_80 = (undefined ***)0x0;
    uStack_78 = 0;
    local_188 = (QTypedArrayData<unsigned_short> *)(CONCAT44(local_188._4_4_,uVar9) | 1);
    plVar5 = (longlong *)thunk_FUN_1401d8ef0(param_1 + 0x1a8,&local_60,*param_2 + 0x10,&local_80);
    uVar9 = uVar9 | 3;
    if (*plVar5 != 0) {
      bVar1 = false;
      goto LAB_1401d7b30;
    }
  }
  bVar1 = true;
LAB_1401d7b30:
  if ((uVar9 & 2) != 0) {
    uVar9 = uVar9 & 0xfffffffd;
    thunk_FUN_1401d4cc0(local_58);
  }
  if ((uVar9 & 1) != 0) {
    uVar9 = uVar9 & 0xfffffffe;
    thunk_FUN_1401d4cc0(uStack_78);
  }
  if (bVar1) {
    uVar3 = thunk_FUN_1401d6990();
    local_188 = QString::fromAscii_helper("grabberAudio",0xc);
    cVar2 = thunk_FUN_1401ce5e0(uVar3,&local_188);
    QString::~QString((QString *)&local_188);
    if (cVar2 != '\0') {
      uVar3 = thunk_FUN_14018fec0(&local_60);
      cVar2 = thunk_FUN_14018fb20(uVar3,30000);
      thunk_FUN_14018e980(&local_60);
      if (cVar2 != '\0') {
        local_178 = &DAT_140351f40;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_f0);
        local_188 = (QTypedArrayData<unsigned_short> *)(CONCAT44(local_188._4_4_,uVar9) | 8);
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_178,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_170,false);
        *(undefined ***)((longlong)&local_178 + (longlong)*(int *)(local_178 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&local_180 + (longlong)*(int *)(local_178 + 4) + 4) =
             *(int *)(local_178 + 4) + -0x88;
        thunk_FUN_1400590e0(&local_170,2);
        this = (QString *)
               (**(code **)(*(longlong *)*param_2 + 0x18))((longlong *)*param_2,&local_80);
        uVar3 = thunk_FUN_140042b60(&local_178,"audio: alien resp received: ");
        this_00 = (QByteArray *)QString::toLocal8Bit(this);
        pcVar6 = QByteArray::data(this_00);
        thunk_FUN_140042b60(uVar3,pcVar6);
        QByteArray::~QByteArray((QByteArray *)&local_188);
        QString::~QString((QString *)&local_80);
        uVar3 = thunk_FUN_14007d360(&local_178,&local_80);
        uVar4 = thunk_FUN_14018fec0(&local_60);
        local_198 = 0x77;
        thunk_FUN_14018f150(uVar4,30000,uVar3,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-audio\\audioSink.cpp"
                           );
        thunk_FUN_14018e980(&local_60);
        if (0xf < local_68) {
          pppuVar8 = local_80;
          if (0xfff < local_68 + 1) {
            if (((ulonglong)local_80 & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            pppuVar8 = (undefined ***)local_80[-1];
            if (local_80 <= pppuVar8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if ((ulonglong)((longlong)local_80 - (longlong)pppuVar8) < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < (ulonglong)((longlong)local_80 - (longlong)pppuVar8)) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_14000219e(pppuVar8);
        }
        thunk_FUN_14005cd10(local_f0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_f0);
      }
    }
  }
  else {
    local_80 = (undefined ***)0x0;
    uStack_78 = 0;
    thunk_FUN_1401d8ef0(param_1 + 0x1a8,&local_188,*param_2 + 0x10,&local_80);
    thunk_FUN_1401d4cc0(uStack_78);
    if (local_188 != (QTypedArrayData<unsigned_short> *)0x0) {
      thunk_FUN_1401e4180();
    }
    thunk_FUN_1401d4cc0(local_180);
  }
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1401d7f00
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1401d7f00(longlong param_1)

{
  char cVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  ulonglong uVar4;
  undefined1 auStack_188 [32];
  undefined4 local_168;
  QTypedArrayData<unsigned_short> *local_158;
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
  local_28 = DAT_1405169d0 ^ (ulonglong)auStack_188;
  local_158 = (QTypedArrayData<unsigned_short> *)((ulonglong)local_158 & 0xffffffff00000000);
  uVar2 = thunk_FUN_1401d6990();
  local_158 = QString::fromAscii_helper("grabberAudio",0xc);
  cVar1 = thunk_FUN_1401ce5e0(uVar2,&local_158);
  QString::~QString((QString *)&local_158);
  if (cVar1 != '\0') {
    uVar2 = thunk_FUN_14018fec0(local_150);
    cVar1 = thunk_FUN_14018fb20(uVar2,10000);
    thunk_FUN_14018e980(local_150);
    if (cVar1 != '\0') {
      local_138 = &DAT_140351f40;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_b0);
      local_158 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_158._4_4_,1);
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
      thunk_FUN_140042b60(&local_138,"audio: AudioSink::recv(const RespAudioStreamFramesPtr &msg)");
      uVar2 = thunk_FUN_14007d360(&local_138,local_48);
      uVar3 = thunk_FUN_14018fec0(local_150);
      local_168 = 0x92;
      thunk_FUN_14018f150(uVar3,10000,uVar2,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-audio\\audioSink.cpp"
                         );
      thunk_FUN_14018e980(local_150);
      if (0xf < local_30) {
        uVar4 = local_48[0];
        if (0xfff < local_30 + 1) {
          if ((local_48[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar4 = *(ulonglong *)(local_48[0] - 8);
          if (local_48[0] <= uVar4) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_48[0] - uVar4 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_48[0] - uVar4) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_14000219e(uVar4);
      }
      *(undefined ***)((longlong)&local_138 + (longlong)*(int *)(local_138 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_140 + (longlong)*(int *)(local_138 + 4) + 4) =
           *(int *)(local_138 + 4) + -0x88;
      local_130 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140065d80(&local_130);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_130);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_128);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_b0);
    }
  }
  *(undefined1 *)(param_1 + 0x1b0) = 0;
  thunk_FUN_1401d86b0(param_1);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1401d8220
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1401d8220(longlong param_1)

{
  char cVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  ulonglong uVar4;
  undefined1 auStack_188 [32];
  undefined4 local_168;
  QTypedArrayData<unsigned_short> *local_158;
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
  local_28 = DAT_1405169d0 ^ (ulonglong)auStack_188;
  local_158 = (QTypedArrayData<unsigned_short> *)((ulonglong)local_158 & 0xffffffff00000000);
  uVar2 = thunk_FUN_1401d6990();
  local_158 = QString::fromAscii_helper("grabberAudio",0xc);
  cVar1 = thunk_FUN_1401ce5e0(uVar2,&local_158);
  QString::~QString((QString *)&local_158);
  if (cVar1 != '\0') {
    uVar2 = thunk_FUN_14018fec0(local_150);
    cVar1 = thunk_FUN_14018fb20(uVar2,10000);
    thunk_FUN_14018e980(local_150);
    if (cVar1 != '\0') {
      local_138 = &DAT_140351f40;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_b0);
      local_158 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_158._4_4_,1);
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
      thunk_FUN_140042b60(&local_138,"audio: AudioSink::recv(const RespAudioStreamsInfoPtr &msg)");
      uVar2 = thunk_FUN_14007d360(&local_138,local_48);
      uVar3 = thunk_FUN_14018fec0(local_150);
      local_168 = 0x87;
      thunk_FUN_14018f150(uVar3,10000,uVar2,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-audio\\audioSink.cpp"
                         );
      thunk_FUN_14018e980(local_150);
      if (0xf < local_30) {
        uVar4 = local_48[0];
        if (0xfff < local_30 + 1) {
          if ((local_48[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar4 = *(ulonglong *)(local_48[0] - 8);
          if (local_48[0] <= uVar4) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_48[0] - uVar4 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_48[0] - uVar4) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_14000219e(uVar4);
      }
      *(undefined ***)((longlong)&local_138 + (longlong)*(int *)(local_138 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_140 + (longlong)*(int *)(local_138 + 4) + 4) =
           *(int *)(local_138 + 4) + -0x88;
      local_130 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140065d80(&local_130);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_130);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_128);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_b0);
    }
  }
  *(undefined1 *)(param_1 + 0x1b0) = 0;
  thunk_FUN_1401d86b0(param_1);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1401d86b0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1401d86b0(longlong param_1)

{
  undefined8 *puVar1;
  char cVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  longlong *plVar5;
  longlong lVar6;
  ulonglong uVar7;
  undefined1 auStack_1b8 [32];
  undefined4 local_198;
  QTypedArrayData<unsigned_short> *local_188 [3];
  undefined8 local_170;
  undefined *local_168;
  basic_streambuf<char,std::char_traits<char>_> local_160 [128];
  basic_ios<char,std::char_traits<char>_> local_e0 [104];
  ulonglong local_78 [3];
  ulonglong local_60;
  longlong local_58 [7];
  longlong *local_20;
  ulonglong local_18;
  
  local_170 = 0xfffffffffffffffe;
  local_18 = DAT_1405169d0 ^ (ulonglong)auStack_1b8;
  local_188[0] = (QTypedArrayData<unsigned_short> *)((ulonglong)local_188[0] & 0xffffffff00000000);
  if (*(longlong *)(param_1 + 0x1d8) == 0) {
    uVar3 = thunk_FUN_1401d6990();
    local_188[0] = QString::fromAscii_helper("grabberAudio",0xc);
    cVar2 = thunk_FUN_1401ce5e0(uVar3,local_188);
    QString::~QString((QString *)local_188);
    if (cVar2 != '\0') {
      uVar3 = thunk_FUN_14018fec0(local_188);
      cVar2 = thunk_FUN_14018fb20(uVar3,10000);
      thunk_FUN_14018e980(local_188);
      if (cVar2 != '\0') {
        local_168 = &DAT_140351f40;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_e0);
        local_188[0] = (QTypedArrayData<unsigned_short> *)CONCAT44(local_188[0]._4_4_,1);
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_168,local_160,false);
        *(undefined ***)(local_160 + (longlong)*(int *)(local_168 + 4) + -8) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&local_170 + (longlong)*(int *)(local_168 + 4) + 4) =
             *(int *)(local_168 + 4) + -0x88;
        thunk_FUN_1400590e0(local_160,2);
        uVar3 = thunk_FUN_140042b60(&local_168,"audio: ");
        uVar3 = thunk_FUN_140042b60(uVar3,"grabber::AudioSink::sendNextStreamMsgToNode");
        thunk_FUN_140042b60(uVar3," - NONE");
        uVar3 = thunk_FUN_14007d360(&local_168,local_78);
        uVar4 = thunk_FUN_14018fec0(local_188);
        local_198 = 0x65;
        thunk_FUN_14018f150(uVar4,10000,uVar3,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-audio\\audioSink.cpp"
                           );
        thunk_FUN_14018e980(local_188);
        if (0xf < local_60) {
          uVar7 = local_78[0];
          if (0xfff < local_60 + 1) {
            if ((local_78[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar7 = *(ulonglong *)(local_78[0] - 8);
            if (local_78[0] <= uVar7) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_78[0] - uVar7 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_78[0] - uVar7) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_14000219e(uVar7);
        }
        thunk_FUN_14005cd10(local_e0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_e0);
      }
    }
  }
  else {
    plVar5 = (longlong *)thunk_FUN_1401d43b0(param_1 + 0x1b8,local_188);
    lVar6 = 0;
    if ((longlong *)*plVar5 != (longlong *)0x0) {
      lVar6 = *(longlong *)*plVar5;
    }
    local_20 = (longlong *)0x0;
    puVar1 = *(undefined8 **)
              (*(longlong *)
                (*(longlong *)(lVar6 + 8) + (*(longlong *)(lVar6 + 0x10) - 1U & plVar5[2]) * 8) +
              0x38);
    if (puVar1 != (undefined8 *)0x0) {
      local_20 = (longlong *)(**(code **)*puVar1)(puVar1,local_58);
    }
    thunk_FUN_1401d6ac0(param_1 + 0x1b8);
    if (local_20 == (longlong *)0x0) {
                    /* WARNING: Subroutine does not return */
      std::_Xbad_function_call();
    }
    (**(code **)(*local_20 + 0x10))();
    *(undefined1 *)(param_1 + 0x1b0) = 1;
    if (local_20 != (longlong *)0x0) {
      (**(code **)(*local_20 + 0x20))
                (local_20,CONCAT71((int7)((ulonglong)local_58 >> 8),local_20 != local_58));
    }
  }
  return;
}

// =============================================================================
// SESIUNI
// =============================================================================

// -----------------------------------------------------------------------------
// Ghidra @ 0x140244bc0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_140244bc0(longlong *param_1)

{
  longlong *plVar1;
  char cVar2;
  undefined4 uVar3;
  undefined2 *puVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  int *piVar7;
  longlong lVar8;
  ulonglong uVar9;
  undefined1 auStack_188 [32];
  undefined4 local_168;
  undefined2 local_160;
  undefined4 local_158 [2];
  QTypedArrayData<unsigned_short> *local_150;
  QString local_148 [8];
  undefined1 local_140 [16];
  undefined8 local_130;
  undefined *local_128;
  basic_streambuf<char,std::char_traits<char>_> local_120 [128];
  basic_ios<char,std::char_traits<char>_> local_a0 [104];
  ulonglong local_38 [3];
  ulonglong local_20;
  ulonglong local_18;
  
  local_130 = 0xfffffffffffffffe;
  local_18 = DAT_1405169d0 ^ (ulonglong)auStack_188;
  local_158[0] = 0;
  plVar1 = param_1 + 0x15;
  cVar2 = thunk_FUN_1402a99e0(plVar1);
  if (cVar2 != '\0') {
    return 1;
  }
  local_150 = QString::fromAscii_helper("Global\\ng9dkwm2s71dkca5umspo_%1_",0x20);
  puVar4 = (undefined2 *)QChar::QChar((QChar *)local_158,0x20);
  local_160 = *puVar4;
  local_168 = 10;
  QString::arg((QString *)&local_150,local_148,(int)param_1[8],0);
  QString::~QString((QString *)&local_150);
  cVar2 = thunk_FUN_1402a83b0(plVar1,local_148);
  if (cVar2 == '\0') {
    uVar3 = thunk_FUN_1402a9540();
    switch(uVar3) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 6:
    case 7:
      break;
    case 5:
switchD_140244ca0_caseD_5:
      Sleep(100);
      break;
    default:
      uVar5 = thunk_FUN_14018fec0(local_140);
      cVar2 = thunk_FUN_14018fb20(uVar5,40000);
      thunk_FUN_14018e980(local_140);
      if (cVar2 != '\0') {
        local_128 = &DAT_140351f40;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_a0);
        local_158[0] = 1;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
        *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&local_130 + (longlong)*(int *)(local_128 + 4) + 4) =
             *(int *)(local_128 + 4) + -0x88;
        thunk_FUN_1400590e0(local_120,2);
        thunk_FUN_140042b60(&local_128,"failed to attach shared segment with error \'unknown\'");
        uVar5 = thunk_FUN_14007d360(&local_128,local_38);
        uVar6 = thunk_FUN_14018fec0(local_140);
        local_168 = 0x7c4;
        thunk_FUN_14018f150(uVar6,40000,uVar5,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-session\\sessionImpl.cpp"
                           );
        thunk_FUN_14018e980(local_140);
        if (0xf < local_20) {
          uVar9 = local_38[0];
          if (0xfff < local_20 + 1) {
            if ((local_38[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar9 = *(ulonglong *)(local_38[0] - 8);
            if (local_38[0] <= uVar9) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_38[0] - uVar9 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_38[0] - uVar9) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_14000219e(uVar9);
        }
        thunk_FUN_14005cd10(local_a0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_a0);
      }
    }
  }
  else {
    cVar2 = thunk_FUN_1402a9a10(plVar1);
    if (cVar2 != '\0') {
      piVar7 = (int *)thunk_FUN_1402a8340(plVar1);
      if ((int)param_1[0x19] != *piVar7) {
        *(undefined4 *)(param_1 + 0x19) = 0;
        lVar8 = thunk_FUN_1402a8340(plVar1);
        *(undefined4 *)(lVar8 + 0x10) = 0;
        thunk_FUN_1402a9a70(plVar1);
        thunk_FUN_1402449a0(param_1 + 2);
        (**(code **)(*param_1 + 0x88))(param_1);
        uVar5 = 1;
        goto LAB_140244e8d;
      }
      thunk_FUN_1402a9a70();
      thunk_FUN_1402a9560(plVar1);
      thunk_FUN_1402449a0(param_1 + 2);
      goto switchD_140244ca0_caseD_5;
    }
    thunk_FUN_1402a9560();
    thunk_FUN_1402449a0(param_1 + 2);
  }
  uVar5 = 0;
LAB_140244e8d:
  QString::~QString(local_148);
  return uVar5;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140249410
// -----------------------------------------------------------------------------
/* FUN_140249410: decompilarea a depasit timeout-ul intern MCP de 5 secunde. */

// -----------------------------------------------------------------------------
// Ghidra @ 0x14024fa00
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14024fa00(QString *param_1,QString *param_2,QString *param_3,QString *param_4)

{
  char cVar1;
  uint uVar2;
  int iVar3;
  DWORD DVar4;
  QString *pQVar5;
  undefined8 uVar6;
  basic_ostream<char,std::char_traits<char>_> *pbVar7;
  undefined8 uVar8;
  ulonglong uVar9;
  uint uVar10;
  undefined1 auStack_1d8 [32];
  undefined1 *local_1b8;
  uint local_1a8 [4];
  undefined8 local_198;
  QString local_190 [8];
  longlong local_188;
  undefined1 local_180 [8];
  undefined8 local_178;
  int iStack_16c;
  undefined *local_168;
  basic_streambuf<char,std::char_traits<char>_> local_160 [128];
  basic_ios<char,std::char_traits<char>_> local_e0 [104];
  ulonglong local_78 [3];
  ulonglong local_60;
  ulonglong local_58;
  
  local_178 = 0xfffffffffffffffe;
  local_58 = DAT_1405169d0 ^ (ulonglong)auStack_1d8;
  uVar10 = 0;
  local_1a8[0] = 0;
  QString::QString((QString *)&local_188);
  QString::QString(local_190);
  if (*(int *)(local_188 + 4) == 0) {
    uVar2 = QString::toUInt(param_1,(bool *)0x0,10);
    local_1b8 = local_180;
    iVar3 = WTSQuerySessionInformationW(0,uVar2,5,&local_198);
    if (iVar3 == 0) {
      uVar6 = thunk_FUN_14018fec0(local_1a8);
      cVar1 = thunk_FUN_14018fb20(uVar6);
      thunk_FUN_14018e980(local_1a8);
      if (cVar1 != '\0') {
        local_168 = &DAT_140351f40;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_e0);
        uVar10 = 1;
        local_1a8[0] = 1;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_168,local_160,false);
        *(undefined ***)(local_160 + (longlong)*(int *)(local_168 + 4) + -8) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_16c + (longlong)*(int *)(local_168 + 4)) =
             *(int *)(local_168 + 4) + -0x88;
        thunk_FUN_1400590e0(local_160,2);
        DVar4 = GetLastError();
        pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
                 thunk_FUN_140042b60(&local_168,
                                     "WTSQuerySessionInformation failed (obtain user name): ");
        std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,DVar4);
        uVar6 = thunk_FUN_14007d360(&local_168,local_78);
        uVar8 = thunk_FUN_14018fec0(local_1a8);
        local_1b8 = (undefined1 *)CONCAT44(local_1b8._4_4_,0xd6);
        thunk_FUN_14018f150(uVar8,40000,uVar6,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-session\\sessionImpl.cpp"
                           );
        thunk_FUN_14018e980(local_1a8);
        if (0xf < local_60) {
          uVar9 = local_78[0];
          if (0xfff < local_60 + 1) {
            if ((local_78[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar9 = *(ulonglong *)(local_78[0] - 8);
            if (local_78[0] <= uVar9) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_78[0] - uVar9 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_78[0] - uVar9) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_14000219e(uVar9);
        }
        thunk_FUN_14005cd10(local_e0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_e0);
      }
    }
    else {
      pQVar5 = (QString *)QString::fromWCharArray((wchar_t *)local_1a8,(int)local_198);
      QString::operator=((QString *)&local_188,pQVar5);
      QString::~QString((QString *)local_1a8);
      WTSFreeMemory(local_198);
      uVar10 = 0;
    }
    uVar2 = QString::toUInt(param_1,(bool *)0x0,10);
    local_1b8 = local_180;
    iVar3 = WTSQuerySessionInformationW(0,uVar2,7,&local_198);
    if (iVar3 == 0) {
      uVar6 = thunk_FUN_14018fec0(local_1a8);
      cVar1 = thunk_FUN_14018fb20(uVar6,40000);
      thunk_FUN_14018e980(local_1a8);
      if (cVar1 != '\0') {
        local_168 = &DAT_140351f40;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_e0);
        local_1a8[0] = uVar10 | 2;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_168,local_160,false);
        *(undefined ***)(local_160 + (longlong)*(int *)(local_168 + 4) + -8) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_16c + (longlong)*(int *)(local_168 + 4)) =
             *(int *)(local_168 + 4) + -0x88;
        thunk_FUN_1400590e0(local_160,2);
        DVar4 = GetLastError();
        pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
                 thunk_FUN_140042b60(&local_168,
                                     "WTSQuerySessionInformation failed (obtain domain name): ");
        std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,DVar4);
        uVar6 = thunk_FUN_14007d360(&local_168,local_78);
        uVar8 = thunk_FUN_14018fec0(local_1a8);
        local_1b8 = (undefined1 *)CONCAT44(local_1b8._4_4_,0xe5);
        thunk_FUN_14018f150(uVar8,40000,uVar6,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-session\\sessionImpl.cpp"
                           );
        thunk_FUN_14018e980(local_1a8);
        if (0xf < local_60) {
          uVar9 = local_78[0];
          if (0xfff < local_60 + 1) {
            if ((local_78[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar9 = *(ulonglong *)(local_78[0] - 8);
            if (local_78[0] <= uVar9) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_78[0] - uVar9 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_78[0] - uVar9) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_14000219e(uVar9);
        }
        thunk_FUN_14005cd10(local_e0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_e0);
      }
    }
    else {
      pQVar5 = (QString *)QString::fromWCharArray((wchar_t *)local_1a8,(int)local_198);
      QString::operator=(local_190,pQVar5);
      QString::~QString((QString *)local_1a8);
      WTSFreeMemory(local_198);
    }
    iVar3 = QString::compare(local_190,param_2,0);
    if (iVar3 == 0) {
      QString::clear(local_190);
    }
  }
  QString::operator=(param_3,(QString *)&local_188);
  QString::operator=(param_4,local_190);
  QString::~QString(local_190);
  QString::~QString((QString *)&local_188);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14024ffd0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14024ffd0(undefined8 param_1,QString *param_2)

{
  char cVar1;
  DWORD DVar2;
  uint uVar3;
  QVariant *this;
  HMODULE hModule;
  FARPROC pFVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  ulonglong uVar7;
  undefined1 auStack_198 [32];
  undefined4 local_178;
  QTypedArrayData<unsigned_short> *local_168;
  QVariant local_160 [16];
  QTypedArrayData<unsigned_short> *local_150;
  QSettings local_148 [12];
  int iStack_13c;
  undefined *local_138;
  basic_streambuf<char,std::char_traits<char>_> local_130 [128];
  basic_ios<char,std::char_traits<char>_> local_b0 [104];
  undefined8 local_48;
  QString *local_40;
  ulonglong local_38 [3];
  ulonglong local_20;
  ulonglong local_18;
  
  local_48 = 0xfffffffffffffffe;
  local_18 = DAT_1405169d0 ^ (ulonglong)auStack_198;
  local_168 = (QTypedArrayData<unsigned_short> *)((ulonglong)local_168 & 0xffffffff00000000);
  local_40 = param_2;
  DVar2 = GetVersion();
  if ((byte)DVar2 < 6) goto LAB_140250578;
  local_150 = QString::fromAscii_helper
                        ("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"
                         ,0x4c);
  QSettings::QSettings(local_148,(QString *)&local_150,0,(QObject *)0x0);
  QString::~QString((QString *)&local_150);
  local_168 = QString::fromAscii_helper("SoftwareSASGeneration",0x15);
  QVariant::QVariant(local_160);
  this = (QVariant *)QSettings::value(local_148,(QString *)local_38,(QVariant *)&local_168);
  uVar3 = QVariant::toUInt(this,(bool *)0x0);
  QVariant::~QVariant((QVariant *)local_38);
  QVariant::~QVariant(local_160);
  QString::~QString((QString *)&local_168);
  if ((uVar3 & 1) == 0) {
    QVariant::QVariant((QVariant *)local_38,uVar3 | 1);
    local_168 = QString::fromAscii_helper("SoftwareSASGeneration",0x15);
    QSettings::setValue(local_148,(QString *)&local_168,(QVariant *)local_38);
    QString::~QString((QString *)&local_168);
    QVariant::~QVariant((QVariant *)local_38);
  }
  hModule = LoadLibraryA("sas.dll");
  if (hModule == (HMODULE)0x0) {
    uVar5 = thunk_FUN_14018fec0(local_160);
    cVar1 = thunk_FUN_14018fb20(uVar5,40000);
    thunk_FUN_14018e980(local_160);
    if (cVar1 != '\0') {
      local_138 = &DAT_140351f40;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_b0);
      local_168 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_168._4_4_,4);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_138,local_130,false);
      *(undefined ***)(local_130 + (longlong)*(int *)(local_138 + 4) + -8) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_13c + (longlong)*(int *)(local_138 + 4)) =
           *(int *)(local_138 + 4) + -0x88;
      thunk_FUN_1400590e0(local_130,2);
      thunk_FUN_140042b60(&local_138,"sas.dll missing, unable to send SAS");
      uVar5 = thunk_FUN_14007d360(&local_138,local_38);
      uVar6 = thunk_FUN_14018fec0(local_160);
      local_178 = 0x1f2;
      thunk_FUN_14018f150(uVar6,40000,uVar5,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-session\\sessionImpl.cpp"
                         );
      thunk_FUN_14018e980(local_160);
      if (0xf < local_20) {
        uVar7 = local_38[0];
        if (0xfff < local_20 + 1) {
          if ((local_38[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar7 = *(ulonglong *)(local_38[0] - 8);
          if (local_38[0] <= uVar7) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_38[0] - uVar7 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_38[0] - uVar7) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_14000219e(uVar7);
      }
LAB_140250558:
      thunk_FUN_14005cd10(local_b0);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_b0);
    }
  }
  else {
    pFVar4 = GetProcAddress(hModule,"SendSAS");
    if (pFVar4 == (FARPROC)0x0) {
      uVar5 = thunk_FUN_14018fec0(local_160);
      cVar1 = thunk_FUN_14018fb20(uVar5,40000);
      thunk_FUN_14018e980(local_160);
      if (cVar1 != '\0') {
        local_138 = &DAT_140351f40;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_b0);
        local_168 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_168._4_4_,2);
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_138,local_130,false);
        *(undefined ***)(local_130 + (longlong)*(int *)(local_138 + 4) + -8) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_13c + (longlong)*(int *)(local_138 + 4)) =
             *(int *)(local_138 + 4) + -0x88;
        thunk_FUN_1400590e0(local_130,2);
        thunk_FUN_140042b60(&local_138,"SendSAS missing in sas.dll, unable to SendSAS");
        uVar5 = thunk_FUN_14007d360(&local_138,local_38);
        uVar6 = thunk_FUN_14018fec0(local_160);
        local_178 = 0x1ed;
        thunk_FUN_14018f150(uVar6,40000,uVar5,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-session\\sessionImpl.cpp"
                           );
        thunk_FUN_14018e980(local_160);
        if (0xf < local_20) {
          uVar7 = local_38[0];
          if (0xfff < local_20 + 1) {
            if ((local_38[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar7 = *(ulonglong *)(local_38[0] - 8);
            if (local_38[0] <= uVar7) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_38[0] - uVar7 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_38[0] - uVar7) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_14000219e(uVar7);
        }
        goto LAB_140250558;
      }
    }
    else {
      uVar5 = thunk_FUN_14018fec0(local_160);
      cVar1 = thunk_FUN_14018fb20(uVar5,20000);
      thunk_FUN_14018e980(local_160);
      if (cVar1 != '\0') {
        local_138 = &DAT_140351f40;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_b0);
        local_168 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_168._4_4_,1);
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_138,local_130,false);
        *(undefined ***)(local_130 + (longlong)*(int *)(local_138 + 4) + -8) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_13c + (longlong)*(int *)(local_138 + 4)) =
             *(int *)(local_138 + 4) + -0x88;
        thunk_FUN_1400590e0(local_130,2);
        thunk_FUN_140042b60(&local_138,"SendSAS");
        uVar5 = thunk_FUN_14007d360(&local_138,local_38);
        uVar6 = thunk_FUN_14018fec0(local_160);
        local_178 = 0x1e8;
        thunk_FUN_14018f150(uVar6,20000,uVar5,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-session\\sessionImpl.cpp"
                           );
        thunk_FUN_14018e980(local_160);
        if (0xf < local_20) {
          uVar7 = local_38[0];
          if (0xfff < local_20 + 1) {
            if ((local_38[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar7 = *(ulonglong *)(local_38[0] - 8);
            if (local_38[0] <= uVar7) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_38[0] - uVar7 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_38[0] - uVar7) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_14000219e(uVar7);
        }
        thunk_FUN_14005cd10(local_b0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_b0);
      }
      (*pFVar4)(0);
    }
  }
  QSettings::~QSettings(local_148);
LAB_140250578:
  QString::~QString(param_2);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140251390
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140251390(longlong param_1)

{
  char cVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  QTypedArrayData<unsigned_short> *pQVar4;
  undefined ***pppuVar5;
  undefined1 auStack_188 [32];
  undefined4 local_168;
  undefined ***local_158 [2];
  undefined8 local_148;
  int iStack_13c;
  undefined *local_138;
  undefined **local_130;
  basic_ostream<char,std::char_traits<char>_> local_128 [96];
  undefined8 local_c8;
  undefined4 local_c0;
  basic_ios<char,std::char_traits<char>_> local_b0 [104];
  QTypedArrayData<unsigned_short> *local_48;
  undefined ***local_40;
  ulonglong local_30;
  ulonglong local_28;
  
  local_148 = 0xfffffffffffffffe;
  local_28 = DAT_1405169d0 ^ (ulonglong)auStack_188;
  local_48 = (QTypedArrayData<unsigned_short> *)((ulonglong)local_48 & 0xffffffff00000000);
  uVar2 = thunk_FUN_140250720();
  local_48 = QString::fromAscii_helper("grabber",7);
  cVar1 = thunk_FUN_1401ce5e0(uVar2,&local_48);
  QString::~QString((QString *)&local_48);
  if (cVar1 != '\0') {
    uVar2 = thunk_FUN_14018fec0(local_158);
    cVar1 = thunk_FUN_14018fb20(uVar2,20000);
    thunk_FUN_14018e980(local_158);
    if (cVar1 != '\0') {
      local_138 = &DAT_140351f40;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_b0);
      local_48 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_48._4_4_,1);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_138,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_130,false);
      *(undefined ***)((longlong)&local_138 + (longlong)*(int *)(local_138 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_13c + (longlong)*(int *)(local_138 + 4)) =
           *(int *)(local_138 + 4) + -0x88;
      local_158[0] = &local_130;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_130);
      local_130 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_c8 = 0;
      local_c0 = 4;
      thunk_FUN_140042b60(&local_138,"grabber send TasksChanged ");
      uVar2 = thunk_FUN_14007d360(&local_138,&local_48);
      uVar3 = thunk_FUN_14018fec0(local_158);
      local_168 = 0x237;
      thunk_FUN_14018f150(uVar3,20000,uVar2,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-session\\sessionImpl.cpp"
                         );
      thunk_FUN_14018e980(local_158);
      if (0xf < local_30) {
        pQVar4 = local_48;
        if (0xfff < local_30 + 1) {
          if (((ulonglong)local_48 & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          pQVar4 = *(QTypedArrayData<unsigned_short> **)(local_48 + -8);
          if (local_48 <= pQVar4) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if ((ulonglong)((longlong)local_48 - (longlong)pQVar4) < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < (ulonglong)((longlong)local_48 - (longlong)pQVar4)) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_14000219e(pQVar4);
      }
      *(undefined ***)((longlong)&local_138 + (longlong)*(int *)(local_138 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_13c + (longlong)*(int *)(local_138 + 4)) =
           *(int *)(local_138 + 4) + -0x88;
      local_130 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140065d80(&local_130);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_130);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_128);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_b0);
    }
  }
  pQVar4 = operator_new(0x10);
  if (pQVar4 == (QTypedArrayData<unsigned_short> *)0x0) {
    pQVar4 = (QTypedArrayData<unsigned_short> *)0x0;
  }
  else {
    *(undefined8 *)(pQVar4 + 8) = 0;
    *(undefined ***)pQVar4 = grabber::SessionImpl::UIData::vftable;
  }
  *(undefined4 *)(pQVar4 + 8) = 0xb9;
  local_48 = pQVar4;
  pppuVar5 = operator_new(0x18);
  if (pppuVar5 + 2 != (undefined ***)0x0) {
    pppuVar5[2] = (undefined **)pQVar4;
  }
  if (pppuVar5 != (undefined ***)0x0) {
    pppuVar5[1] = (undefined **)&LAB_14000b9c4;
    *(undefined4 *)((longlong)pppuVar5 + 4) = 1;
    *(undefined4 *)pppuVar5 = 1;
  }
  local_158[0] = pppuVar5;
  local_40 = pppuVar5;
  thunk_FUN_14007cde0(pppuVar5,pQVar4,1);
  FUN_1400055c9(&local_48,pQVar4);
  thunk_FUN_140244750(param_1 + 0x20,&local_48);
  thunk_FUN_140246220(pppuVar5);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140251760
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140251760(longlong param_1,QString *param_2)

{
  char cVar1;
  undefined8 uVar2;
  QByteArray *this;
  char *pcVar3;
  undefined8 uVar4;
  undefined **ppuVar5;
  undefined ***pppuVar6;
  undefined1 auStack_198 [32];
  undefined4 local_178;
  undefined ***local_168 [2];
  undefined8 local_158;
  int iStack_14c;
  undefined *local_148;
  undefined **local_140;
  basic_ostream<char,std::char_traits<char>_> local_138 [96];
  undefined8 local_d8;
  undefined4 local_d0;
  basic_ios<char,std::char_traits<char>_> local_c0 [104];
  undefined **local_58;
  undefined ***local_50;
  ulonglong local_40;
  ulonglong local_38;
  
  local_158 = 0xfffffffffffffffe;
  local_38 = DAT_1405169d0 ^ (ulonglong)auStack_198;
  local_58 = (undefined **)((ulonglong)local_58 & 0xffffffff00000000);
  uVar2 = thunk_FUN_14018fec0(local_168);
  cVar1 = thunk_FUN_14018fb20(uVar2);
  thunk_FUN_14018e980(local_168);
  if (cVar1 != '\0') {
    local_148 = &DAT_140351f40;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_c0);
    local_58 = (undefined **)CONCAT44(local_58._4_4_,1);
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
    uVar2 = thunk_FUN_140042b60(&local_148,"grabber send webtoken ");
    this = (QByteArray *)QString::toLocal8Bit(param_2);
    pcVar3 = QByteArray::data(this);
    thunk_FUN_140042b60(uVar2,pcVar3);
    QByteArray::~QByteArray((QByteArray *)&local_58);
    uVar2 = thunk_FUN_14007d360(&local_148,&local_58);
    uVar4 = thunk_FUN_14018fec0(local_168);
    local_178 = 0x22a;
    thunk_FUN_14018f150(uVar4,20000,uVar2,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-session\\sessionImpl.cpp"
                       );
    thunk_FUN_14018e980(local_168);
    if (0xf < local_40) {
      ppuVar5 = local_58;
      if (0xfff < local_40 + 1) {
        if (((ulonglong)local_58 & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        ppuVar5 = (undefined **)local_58[-1];
        if (local_58 <= ppuVar5) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if ((ulonglong)((longlong)local_58 - (longlong)ppuVar5) < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < (ulonglong)((longlong)local_58 - (longlong)ppuVar5)) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_14000219e(ppuVar5);
    }
    *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
         *(int *)(local_148 + 4) + -0x88;
    local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_140065d80(&local_140);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_138);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_c0)
    ;
  }
  ppuVar5 = operator_new(0x410);
  if (ppuVar5 == (undefined **)0x0) {
    ppuVar5 = (undefined **)0x0;
  }
  else {
    memset(ppuVar5,0,0x410);
    *ppuVar5 = (undefined *)
               grabber::SessionImpl::UIDataDerived<grabber::CD_UISelfControlWebToken>::vftable;
  }
  *(undefined4 *)(ppuVar5 + 1) = 0xb8;
  thunk_FUN_1402401c0(ppuVar5 + 2,param_2);
  local_58 = ppuVar5;
  pppuVar6 = operator_new(0x18);
  if (pppuVar6 + 2 != (undefined ***)0x0) {
    pppuVar6[2] = ppuVar5;
  }
  if (pppuVar6 != (undefined ***)0x0) {
    pppuVar6[1] = (undefined **)&LAB_14000b9c4;
    *(undefined4 *)((longlong)pppuVar6 + 4) = 1;
    *(undefined4 *)pppuVar6 = 1;
  }
  local_168[0] = pppuVar6;
  local_50 = pppuVar6;
  thunk_FUN_14007cde0(pppuVar6,ppuVar5,1);
  FUN_1400055c9(&local_58,ppuVar5);
  thunk_FUN_140244750(param_1 + 0x20,&local_58);
  thunk_FUN_140246220(pppuVar6);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1402521e0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

ulonglong FUN_1402521e0(longlong *param_1)

{
  uint uVar1;
  char cVar2;
  BOOL BVar3;
  int iVar4;
  DWORD DVar5;
  undefined4 extraout_var;
  undefined8 uVar6;
  basic_ostream<char,std::char_traits<char>_> *pbVar7;
  undefined8 uVar8;
  HLOCAL pvVar9;
  HANDLE ProcessHandle;
  undefined4 extraout_var_00;
  undefined4 extraout_var_01;
  QString *pQVar10;
  QFileInfo *this;
  longlong *plVar11;
  QTypedArrayData<unsigned_short> **ppQVar12;
  LPCWSTR lpCurrentDirectory;
  HLOCAL extraout_RAX;
  ulonglong uVar13;
  LPCWSTR lpApplicationName;
  WCHAR *lpCommandLine;
  bool bVar14;
  undefined1 auStackY_438 [32];
  undefined ***local_3b8;
  char *pcStack_3b0;
  undefined1 local_3a8 [8];
  HANDLE local_3a0;
  QTypedArrayData<unsigned_short> *local_398;
  undefined8 uStack_390;
  QString local_388 [8];
  QString local_380 [8];
  uint *local_378;
  undefined8 uStack_370;
  undefined ***local_368;
  undefined8 uStack_360;
  undefined *local_358;
  undefined **local_350;
  basic_ostream<char,std::char_traits<char>_> local_348 [96];
  undefined8 local_2e8;
  undefined4 local_2e0;
  basic_ios<char,std::char_traits<char>_> local_2d0 [104];
  DWORD local_268 [4];
  ulong local_258 [2];
  QString local_250 [8];
  QDir local_248 [8];
  QString local_240 [8];
  QString local_238 [8];
  QString local_230 [8];
  QString local_228 [8];
  QString local_220 [8];
  _PROCESS_INFORMATION local_218;
  QString local_200 [8];
  QString local_1f8 [8];
  QString local_1f0 [8];
  QString local_1e8 [8];
  QString local_1e0 [8];
  undefined8 local_1d8;
  undefined8 local_1c8;
  undefined8 uStack_1c0;
  undefined8 local_1b8;
  undefined8 uStack_1b0;
  undefined8 local_1a8;
  undefined8 uStack_1a0;
  undefined8 local_198;
  undefined8 uStack_190;
  _STARTUPINFOW local_188;
  ulonglong local_118 [3];
  ulonglong local_100;
  WCHAR local_f8;
  undefined6 uStack_f6;
  longlong local_e8;
  ulonglong local_e0;
  WCHAR local_d8;
  undefined6 uStack_d6;
  longlong local_c8;
  ulonglong local_c0;
  WCHAR local_b8;
  undefined6 uStack_b6;
  longlong local_a8;
  ulonglong local_a0;
  undefined8 local_98;
  wchar_t awStack_90 [8];
  wchar_t local_80;
  undefined6 uStack_7e;
  undefined2 uStack_78;
  undefined4 local_76;
  undefined2 local_72;
  ulonglong local_68;
  
  local_1d8 = 0xfffffffffffffffe;
  local_68 = DAT_1405169d0 ^ (ulonglong)auStackY_438;
  lpApplicationName = (LPCWSTR)0x0;
  if ((HANDLE)param_1[7] != (HANDLE)0xffffffffffffffff) {
    BVar3 = GetExitCodeProcess((HANDLE)param_1[7],local_268);
    if (BVar3 != 0) {
      if (local_268[0] == 0x103) {
        return CONCAT71((int7)(CONCAT44(extraout_var,BVar3) >> 8),1);
      }
      uVar6 = thunk_FUN_14018fec0(&local_3b8);
      cVar2 = thunk_FUN_14018fb20(uVar6);
      thunk_FUN_14018e980(&local_3b8);
      if (cVar2 != '\0') {
        local_358 = &DAT_140351f40;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_2d0);
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_358,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_350,false);
        *(undefined ***)((longlong)&local_358 + (longlong)*(int *)(local_358 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&uStack_360 + (longlong)*(int *)(local_358 + 4) + 4) =
             *(int *)(local_358 + 4) + -0x88;
        thunk_FUN_1400590e0(&local_350,2);
        pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
                 thunk_FUN_140042b60(&local_358,"agent terminated with code ");
        std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,local_268[0]);
        uVar6 = thunk_FUN_14007d360(&local_358,local_118);
        uVar8 = thunk_FUN_14018fec0(&local_3b8);
        thunk_FUN_14018f150(uVar8,20000,uVar6,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-session\\sessionImpl.cpp"
                           );
        thunk_FUN_14018e980(&local_3b8);
        if (0xf < local_100) {
          uVar13 = local_118[0];
          if (0xfff < local_100 + 1) {
            if ((local_118[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar13 = *(ulonglong *)(local_118[0] - 8);
            if (local_118[0] <= uVar13) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_118[0] - uVar13 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_118[0] - uVar13) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_14000219e(uVar13);
        }
        thunk_FUN_14005cd10(local_2d0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_2d0);
      }
      CloseHandle((HANDLE)param_1[7]);
      param_1[7] = -1;
      *(undefined4 *)(param_1 + 8) = 0;
    }
  }
  thunk_FUN_140253b30(param_1);
  local_3a0 = (HANDLE)0x0;
  local_258[0] = QString::toULong((QString *)(param_1 + 6),(bool *)0x0,10);
  iVar4 = WTSQuerySessionInformationW(0,local_258[0],8,&local_378);
  if (iVar4 == 0) {
    uVar6 = thunk_FUN_14018fec0(local_118);
    cVar2 = thunk_FUN_14018fb20(uVar6,30000);
    pvVar9 = (HLOCAL)thunk_FUN_14018e980(local_118);
    if (cVar2 == '\0') goto LAB_1402535e0;
    local_358 = &DAT_140351f40;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_2d0)
    ;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_358,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_350,false);
    *(undefined ***)((longlong)&local_358 + (longlong)*(int *)(local_358 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&uStack_360 + (longlong)*(int *)(local_358 + 4) + 4) =
         *(int *)(local_358 + 4) + -0x88;
    local_3b8 = &local_350;
    std::basic_streambuf<char,std::char_traits<char>_>::
    basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_350);
    local_350 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    local_2e8 = 0;
    local_2e0 = 4;
    DVar5 = GetLastError();
    pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
             thunk_FUN_140042b60(&local_358,"WTSQuerySessionInformationW: ");
    std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,DVar5);
    uVar6 = thunk_FUN_14007d360(&local_358,&local_98);
    uVar8 = thunk_FUN_14018fec0(local_118);
    thunk_FUN_14018f150(uVar8,30000,uVar6,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-session\\sessionImpl.cpp"
                       );
    thunk_FUN_14018e980(local_118);
    if (0xf < CONCAT62(uStack_7e,local_80)) {
      uVar13 = local_98;
      if (0xfff < CONCAT62(uStack_7e,local_80) + 1) {
        if ((local_98 & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar13 = *(ulonglong *)(local_98 - 8);
        if (local_98 <= uVar13) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_98 - uVar13 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_98 - uVar13) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_14000219e(uVar13);
    }
    *(undefined ***)((longlong)&local_358 + (longlong)*(int *)(local_358 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&uStack_360 + (longlong)*(int *)(local_358 + 4) + 4) =
         *(int *)(local_358 + 4) + -0x88;
    local_350 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_140065d80(&local_350);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_350);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_348);
  }
  else {
    uVar1 = *local_378;
    pvVar9 = (HLOCAL)WTSFreeMemory();
    if (((uVar1 & 0xfffffffa) != 0) || (uVar1 == 5)) goto LAB_1402535e0;
    ProcessHandle = GetCurrentProcess();
    BVar3 = OpenProcessToken(ProcessHandle,0xf01ff,&local_398);
    if (BVar3 != 0) {
      BVar3 = DuplicateTokenEx(local_398,0x2000000,(LPSECURITY_ATTRIBUTES)0x0,SecurityImpersonation,
                               TokenPrimary,&local_3a0);
      if (BVar3 == 0) {
        uVar6 = thunk_FUN_14018fec0(&local_3b8);
        cVar2 = thunk_FUN_14018fb20(uVar6,40000);
        thunk_FUN_14018e980(&local_3b8);
        if (cVar2 != '\0') {
          local_358 = &DAT_140351f40;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_2d0);
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_358,
                     (basic_streambuf<char,std::char_traits<char>_> *)&local_350,false);
          *(undefined ***)((longlong)&local_358 + (longlong)*(int *)(local_358 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&uStack_360 + (longlong)*(int *)(local_358 + 4) + 4) =
               *(int *)(local_358 + 4) + -0x88;
          thunk_FUN_1400590e0(&local_350,2);
          DVar5 = GetLastError();
          pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
                   thunk_FUN_140042b60(&local_358,"DuplicateTokenEx: ");
          std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,DVar5);
          uVar6 = thunk_FUN_14007d360(&local_358,local_118);
          uVar8 = thunk_FUN_14018fec0(&local_3b8);
          thunk_FUN_14018f150(uVar8,40000,uVar6,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-session\\sessionImpl.cpp"
                             );
          thunk_FUN_14018e980(&local_3b8);
          if (0xf < local_100) {
            uVar13 = local_118[0];
            if (0xfff < local_100 + 1) {
              if ((local_118[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              uVar13 = *(ulonglong *)(local_118[0] - 8);
              if (local_118[0] <= uVar13) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (local_118[0] - uVar13 < 8) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (0x27 < local_118[0] - uVar13) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
            }
            FUN_14000219e(uVar13);
          }
          thunk_FUN_14005cd10(local_2d0);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_2d0);
        }
        BVar3 = CloseHandle(local_398);
        pvVar9 = (HLOCAL)CONCAT44(extraout_var_00,BVar3);
        goto LAB_1402535e0;
      }
      CloseHandle(local_398);
      BVar3 = SetTokenInformation(local_3a0,TokenSessionId,local_258,4);
      if (BVar3 == 0) {
        uVar6 = thunk_FUN_14018fec0(&local_3b8);
        cVar2 = thunk_FUN_14018fb20(uVar6,40000);
        thunk_FUN_14018e980(&local_3b8);
        if (cVar2 != '\0') {
          local_358 = &DAT_140351f40;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_2d0);
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_358,
                     (basic_streambuf<char,std::char_traits<char>_> *)&local_350,false);
          *(undefined ***)((longlong)&local_358 + (longlong)*(int *)(local_358 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&uStack_360 + (longlong)*(int *)(local_358 + 4) + 4) =
               *(int *)(local_358 + 4) + -0x88;
          thunk_FUN_1400590e0(&local_350,2);
          DVar5 = GetLastError();
          pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
                   thunk_FUN_140042b60(&local_358,"SetTokenInformation: ");
          std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,DVar5);
          uVar6 = thunk_FUN_14007d360(&local_358,local_118);
          uVar8 = thunk_FUN_14018fec0(&local_3b8);
          thunk_FUN_14018f150(uVar8,40000,uVar6,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-session\\sessionImpl.cpp"
                             );
          thunk_FUN_14018e980(&local_3b8);
          thunk_FUN_14005cdb0(local_118);
LAB_1402528a3:
          thunk_FUN_14005cd10(local_2d0);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_2d0);
        }
      }
      else {
        DVar5 = GetVersion();
        if ((byte)DVar5 < 6) {
LAB_140252b9f:
          memset(&local_188,0,0x68);
          local_188.cb = 0x68;
          local_188.dwFlags = 1;
          local_188.wShowWindow = 5;
          local_218.hProcess = (HANDLE)0x0;
          local_218.hThread = (HANDLE)0x0;
          local_218.dwProcessId = 0;
          local_218.dwThreadId = 0;
          pQVar10 = (QString *)QCoreApplication::applicationDirPath();
          QDir::QDir(local_248,pQVar10);
          QString::~QString((QString *)&local_378);
          QDir::absolutePath(local_248);
          QDir::toNativeSeparators(local_220);
          QString::~QString((QString *)&local_398);
          pQVar10 = (QString *)QCoreApplication::applicationFilePath();
          this = (QFileInfo *)QFileInfo::QFileInfo((QFileInfo *)&local_378,pQVar10);
          QFileInfo::fileName(this);
          QFileInfo::~QFileInfo((QFileInfo *)&local_378);
          QString::~QString((QString *)&local_368);
          uVar6 = QString::QString((QString *)&local_398,local_228);
          build_component_profile(local_200,uVar6);
          thunk_FUN_140244100(local_200,local_250);
          QDir::absoluteFilePath(local_248,(QString *)&local_378);
          QDir::toNativeSeparators(local_230);
          QString::~QString((QString *)&local_378);
          plVar11 = (longlong *)(**(code **)(*param_1 + 0x70))(param_1);
          bVar14 = *(int *)(*plVar11 + 4) != 0;
          if (bVar14) {
            uVar6 = (**(code **)(*param_1 + 0x70))(param_1,local_388);
            ppQVar12 = (QTypedArrayData<unsigned_short> **)
                       thunk_FUN_1400611d0(local_268,&DAT_14042f720,uVar6);
          }
          else {
            local_398 = QString::fromAscii_helper("",0);
            ppQVar12 = &local_398;
          }
          uVar6 = (**(code **)(*param_1 + 0x68))(param_1,&local_368);
          thunk_FUN_1400610d0(local_238,uVar6,ppQVar12);
          QString::~QString((QString *)&local_368);
          if (bVar14) {
            QString::~QString((QString *)local_268);
            QString::~QString(local_388);
          }
          else {
            QString::~QString((QString *)&local_398);
          }
          QString::~QString((QString *)&local_3b8);
          local_3a8[0] = 0;
          pQVar10 = (QString *)FUN_1402433d0(local_3a8,&local_378);
          QString::arg(pQVar10,local_240,local_250);
          QString::~QString((QString *)&local_378);
          QString::toStdWString(local_240);
          QString::toStdWString(local_230);
          QString::toStdWString(local_220);
          lpCurrentDirectory = lpApplicationName;
          if (local_e8 != 0) {
            lpCurrentDirectory = &local_f8;
            if (7 < local_e0) {
              lpCurrentDirectory = (LPCWSTR)CONCAT62(uStack_f6,local_f8);
            }
          }
          lpCommandLine = lpApplicationName;
          if (local_a8 != 0) {
            lpCommandLine = &local_b8;
            if (7 < local_a0) {
              lpCommandLine = (LPWSTR)CONCAT62(uStack_b6,local_b8);
            }
          }
          if (local_c8 != 0) {
            lpApplicationName = &local_d8;
            if (7 < local_c0) {
              lpApplicationName = (LPCWSTR)CONCAT62(uStack_d6,local_d8);
            }
          }
          BVar3 = CreateProcessAsUserW
                            (local_3a0,lpApplicationName,lpCommandLine,(LPSECURITY_ATTRIBUTES)0x0,
                             (LPSECURITY_ATTRIBUTES)0x0,0,0x220,(LPVOID)0x0,lpCurrentDirectory,
                             &local_188,&local_218);
          if (BVar3 == 0) {
            uVar6 = thunk_FUN_14018fec0(&local_3b8);
            cVar2 = thunk_FUN_14018fb20(uVar6,40000);
            thunk_FUN_14018e980(&local_3b8);
            if (cVar2 != '\0') {
              local_358 = &DAT_140351f40;
              std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                        (local_2d0);
              std::basic_ostream<char,std::char_traits<char>_>::
              basic_ostream<char,std::char_traits<char>_>
                        ((basic_ostream<char,std::char_traits<char>_> *)&local_358,
                         (basic_streambuf<char,std::char_traits<char>_> *)&local_350,false);
              *(undefined ***)((longlong)&local_358 + (longlong)*(int *)(local_358 + 4)) =
                   std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                   vftable;
              *(int *)((longlong)&uStack_360 + (longlong)*(int *)(local_358 + 4) + 4) =
                   *(int *)(local_358 + 4) + -0x88;
              thunk_FUN_1400590e0(&local_350,2);
              DVar5 = GetLastError();
              pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
                       thunk_FUN_140042b60(&local_358,"CreateProcessAsUser: ");
              std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,DVar5);
              uVar6 = thunk_FUN_14007d360(&local_358,local_118);
              uVar8 = thunk_FUN_14018fec0(&local_3b8);
              thunk_FUN_14018f150(uVar8,40000,uVar6,
                                  "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-session\\sessionImpl.cpp"
                                 );
              thunk_FUN_14018e980(&local_3b8);
              thunk_FUN_14005cdb0(local_118);
              thunk_FUN_14005cd10(local_2d0);
              std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                        (local_2d0);
            }
            CloseHandle(local_3a0);
            uVar13 = 0;
          }
          else {
            WaitForInputIdle(local_218.hProcess,1000);
            Sleep(100);
            CloseHandle(local_188.hStdInput);
            CloseHandle(local_188.hStdOutput);
            CloseHandle(local_188.hStdError);
            CloseHandle(local_3a0);
            CloseHandle(local_218.hThread);
            param_1[7] = (longlong)local_218.hProcess;
            *(DWORD *)(param_1 + 8) = local_218.dwProcessId;
            uVar6 = thunk_FUN_14018fec0(&local_3b8);
            cVar2 = thunk_FUN_14018fb20(uVar6,20000);
            thunk_FUN_14018e980(&local_3b8);
            if (cVar2 != '\0') {
              local_358 = &DAT_140351f40;
              std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                        (local_2d0);
              std::basic_ostream<char,std::char_traits<char>_>::
              basic_ostream<char,std::char_traits<char>_>
                        ((basic_ostream<char,std::char_traits<char>_> *)&local_358,
                         (basic_streambuf<char,std::char_traits<char>_> *)&local_350,false);
              *(undefined ***)((longlong)&local_358 + (longlong)*(int *)(local_358 + 4)) =
                   std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                   vftable;
              *(int *)((longlong)&uStack_360 + (longlong)*(int *)(local_358 + 4) + 4) =
                   *(int *)(local_358 + 4) + -0x88;
              thunk_FUN_1400590e0(&local_350,2);
              pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
                       thunk_FUN_140042b60(&local_358,"agent started with pid = ");
              std::basic_ostream<char,std::char_traits<char>_>::operator<<
                        (pbVar7,*(ulong *)(param_1 + 8));
              uVar6 = thunk_FUN_14007d360(&local_358,local_118);
              uVar8 = thunk_FUN_14018fec0(&local_3b8);
              thunk_FUN_14018f150(uVar8,20000,uVar6,
                                  "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-session\\sessionImpl.cpp"
                                 );
              thunk_FUN_14018e980(&local_3b8);
              thunk_FUN_14005cdb0(local_118);
              thunk_FUN_14005cd10(local_2d0);
              std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                        (local_2d0);
            }
            local_3b8 = (undefined ***)thunk_FUN_1400e9b20(local_388,param_1 + 0xd,param_1 + 0xe);
            local_378 = (uint *)0x0;
            uStack_370 = 0;
            local_398 = (QTypedArrayData<unsigned_short> *)0x0;
            uStack_390 = 0;
            local_368 = (undefined ***)0x0;
            uStack_360 = 0;
            local_268[0] = 0;
            local_268[1] = 0;
            local_268[2] = 0;
            local_268[3] = 0;
            local_1c8 = 0;
            uStack_1c0 = 0;
            local_1b8 = 0;
            uStack_1b0 = 0;
            local_1a8 = 0;
            uStack_1a0 = 0;
            local_198 = 0;
            uStack_190 = 0;
            local_98 = 0;
            awStack_90[0] = L'\0';
            awStack_90[1] = L'\0';
            awStack_90[2] = L'\0';
            awStack_90[3] = L'\0';
            pcStack_3b0 = "PSessionUser";
            local_118[0] = 0;
            local_118[1] = 0;
            QMetaObject::invokeMethod(param_1,"agentStarted",2,local_118);
            QString::~QString(local_380);
            QString::~QString(local_388);
            uVar13 = 1;
          }
          if (7 < local_e0) {
            thunk_FUN_14008e880(&local_f8,CONCAT62(uStack_f6,local_f8),local_e0 + 1);
          }
          local_e0 = 7;
          local_e8 = 0;
          local_f8 = L'\0';
          if (7 < local_c0) {
            thunk_FUN_14008e880(&local_d8,CONCAT62(uStack_d6,local_d8),local_c0 + 1);
          }
          local_c0 = 7;
          local_c8 = 0;
          local_d8 = L'\0';
          if (7 < local_a0) {
            thunk_FUN_14008e880(&local_b8,CONCAT62(uStack_b6,local_b8),local_a0 + 1);
          }
          local_a0 = 7;
          local_a8 = 0;
          local_b8 = L'\0';
          QString::~QString(local_240);
          QString::~QString(local_238);
          QString::~QString(local_230);
          QString::~QString(local_250);
          QString::~QString(local_1e0);
          QString::~QString(local_1e8);
          QString::~QString(local_1f0);
          QString::~QString(local_1f8);
          QString::~QString(local_200);
          QString::~QString(local_228);
          QString::~QString(local_220);
          QDir::~QDir(local_248);
          return uVar13;
        }
        local_98 = CONCAT44(u_S_1_16_16384_140444b20._4_4_,u_S_1_16_16384_140444b20._0_4_);
        awStack_90[2] = u_S_1_16_16384_140444b20[6];
        awStack_90[3] = u_S_1_16_16384_140444b20[7];
        awStack_90[0] = u_S_1_16_16384_140444b20[4];
        awStack_90[1] = u_S_1_16_16384_140444b20[5];
        awStack_90[4] = u_S_1_16_16384_140444b20[8];
        awStack_90[5] = u_S_1_16_16384_140444b20[9];
        awStack_90[6] = u_S_1_16_16384_140444b20[10];
        awStack_90[7] = u_S_1_16_16384_140444b20[0xb];
        local_80 = u_S_1_16_16384_140444b20[0xc];
        uStack_7e = 0;
        uStack_78 = 0;
        local_76 = 0;
        local_72 = 0;
        local_368 = (undefined ***)0x0;
        iVar4 = ConvertStringSidToSidW(&local_98,&local_368);
        if (iVar4 != 0) {
          pcStack_3b0 = (char *)0x20;
          local_3b8 = local_368;
          DVar5 = GetLengthSid(local_368);
          BVar3 = SetTokenInformation(local_3a0,TokenIntegrityLevel,&local_3b8,DVar5 + 0x10);
          if (BVar3 == 0) {
            uVar6 = thunk_FUN_14018fec0(local_388);
            cVar2 = thunk_FUN_14018fb20(uVar6,40000);
            thunk_FUN_14018e980(local_388);
            if (cVar2 != '\0') {
              local_358 = &DAT_140351f40;
              std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                        (local_2d0);
              std::basic_ostream<char,std::char_traits<char>_>::
              basic_ostream<char,std::char_traits<char>_>
                        ((basic_ostream<char,std::char_traits<char>_> *)&local_358,
                         (basic_streambuf<char,std::char_traits<char>_> *)&local_350,false);
              *(undefined ***)((longlong)&local_358 + (longlong)*(int *)(local_358 + 4)) =
                   std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                   vftable;
              *(int *)((longlong)&uStack_360 + (longlong)*(int *)(local_358 + 4) + 4) =
                   *(int *)(local_358 + 4) + -0x88;
              thunk_FUN_1400590e0(&local_350,2);
              DVar5 = GetLastError();
              pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
                       thunk_FUN_140042b60(&local_358,"SetTokenInformation (il): ");
              std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,DVar5);
              uVar6 = thunk_FUN_14007d360(&local_358,local_118);
              uVar8 = thunk_FUN_14018fec0(local_388);
              thunk_FUN_14018f150(uVar8,40000,uVar6,
                                  "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-session\\sessionImpl.cpp"
                                 );
              thunk_FUN_14018e980(local_388);
              thunk_FUN_14005cdb0(local_118);
              thunk_FUN_14005cd10(local_2d0);
              std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                        (local_2d0);
            }
            CloseHandle(local_3a0);
            pvVar9 = LocalFree(local_368);
            goto LAB_1402535e0;
          }
          LocalFree(local_368);
          goto LAB_140252b9f;
        }
        uVar6 = thunk_FUN_14018fec0(local_388);
        cVar2 = thunk_FUN_14018fb20(uVar6,40000);
        thunk_FUN_14018e980(local_388);
        if (cVar2 != '\0') {
          local_358 = &DAT_140351f40;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_2d0);
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_358,
                     (basic_streambuf<char,std::char_traits<char>_> *)&local_350,false);
          *(undefined ***)((longlong)&local_358 + (longlong)*(int *)(local_358 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&uStack_360 + (longlong)*(int *)(local_358 + 4) + 4) =
               *(int *)(local_358 + 4) + -0x88;
          thunk_FUN_1400590e0(&local_350,2);
          DVar5 = GetLastError();
          pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
                   thunk_FUN_140042b60(&local_358,"ConvertStringSidToSid (il): ");
          std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,DVar5);
          uVar6 = thunk_FUN_14007d360(&local_358,local_118);
          uVar8 = thunk_FUN_14018fec0(local_388);
          thunk_FUN_14018f150(uVar8,40000,uVar6,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-session\\sessionImpl.cpp"
                             );
          thunk_FUN_14018e980(local_388);
          thunk_FUN_14005cdb0(local_118);
          goto LAB_1402528a3;
        }
      }
      BVar3 = CloseHandle(local_3a0);
      pvVar9 = (HLOCAL)CONCAT44(extraout_var_01,BVar3);
      goto LAB_1402535e0;
    }
    uVar6 = thunk_FUN_14018fec0(&local_3b8);
    cVar2 = thunk_FUN_14018fb20(uVar6,40000);
    pvVar9 = (HLOCAL)thunk_FUN_14018e980(&local_3b8);
    if (cVar2 == '\0') goto LAB_1402535e0;
    local_358 = &DAT_140351f40;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_2d0)
    ;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_358,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_350,false);
    *(undefined ***)((longlong)&local_358 + (longlong)*(int *)(local_358 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&uStack_360 + (longlong)*(int *)(local_358 + 4) + 4) =
         *(int *)(local_358 + 4) + -0x88;
    thunk_FUN_1400590e0(&local_350,2);
    DVar5 = GetLastError();
    pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
             thunk_FUN_140042b60(&local_358,"OpenProcessToken: ");
    std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,DVar5);
    uVar6 = thunk_FUN_14007d360(&local_358,local_118);
    uVar8 = thunk_FUN_14018fec0(&local_3b8);
    thunk_FUN_14018f150(uVar8,40000,uVar6,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-session\\sessionImpl.cpp"
                       );
    thunk_FUN_14018e980(&local_3b8);
    if (0xf < local_100) {
      uVar13 = local_118[0];
      if (0xfff < local_100 + 1) {
        if ((local_118[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar13 = *(ulonglong *)(local_118[0] - 8);
        if (local_118[0] <= uVar13) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_118[0] - uVar13 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_118[0] - uVar13) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_14000219e(uVar13);
    }
    thunk_FUN_14005cd10(local_2d0);
  }
  std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_2d0);
  pvVar9 = extraout_RAX;
LAB_1402535e0:
  return (ulonglong)pvVar9 & 0xffffffffffffff00;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140253b30
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_140253b30(longlong param_1)

{
  char cVar1;
  BOOL BVar2;
  longlong lVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  ulonglong uVar6;
  uint uVar7;
  undefined1 auStack_198 [32];
  undefined4 local_178;
  uint local_168;
  undefined ***local_160 [2];
  DWORD local_150;
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
  local_30 = DAT_1405169d0 ^ (ulonglong)auStack_198;
  uVar7 = 0;
  local_168 = 0;
  cVar1 = thunk_FUN_1402a99e0(param_1 + 0xa8);
  if (cVar1 != '\0') {
    thunk_FUN_1402a9a10(param_1 + 0xa8);
    lVar3 = thunk_FUN_1402a8340(param_1 + 0xa8);
    *(undefined4 *)(lVar3 + 0x10) = 0xb;
    thunk_FUN_1402a9a70(param_1 + 0xa8);
  }
  thunk_FUN_1402a9560(param_1 + 0xa8);
  thunk_FUN_1402449a0(param_1 + 0x10);
  if (*(longlong *)(param_1 + 0x38) != -1) {
    uVar4 = thunk_FUN_14018fec0(local_160);
    cVar1 = thunk_FUN_14018fb20(uVar4,20000);
    thunk_FUN_14018e980(local_160);
    if (cVar1 != '\0') {
      local_148 = &DAT_140351f40;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_c0);
      uVar7 = 1;
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
      thunk_FUN_140042b60(&local_148,"wait agent termination");
      uVar4 = thunk_FUN_14007d360(&local_148,local_50);
      uVar5 = thunk_FUN_14018fec0(local_160);
      local_178 = 0x759;
      thunk_FUN_14018f150(uVar5,20000,uVar4,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-session\\sessionImpl.cpp"
                         );
      thunk_FUN_14018e980(local_160);
      if (0xf < local_38) {
        uVar6 = local_50[0];
        if (0xfff < local_38 + 1) {
          if ((local_50[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar6 = *(ulonglong *)(local_50[0] - 8);
          if (local_50[0] <= uVar6) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_50[0] - uVar6 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_50[0] - uVar6) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_14000219e(uVar6);
      }
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140065d80(&local_140);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_138);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_c0);
    }
    WaitForSingleObject(*(HANDLE *)(param_1 + 0x38),100);
    BVar2 = GetExitCodeProcess(*(HANDLE *)(param_1 + 0x38),&local_150);
    if ((BVar2 != 0) && (local_150 == 0x103)) {
      uVar4 = thunk_FUN_14018fec0(local_160);
      cVar1 = thunk_FUN_14018fb20(uVar4,20000);
      thunk_FUN_14018e980(local_160);
      if (cVar1 != '\0') {
        local_148 = &DAT_140351f40;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_c0);
        local_168 = uVar7 | 2;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_148,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_140,false);
        *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
             *(int *)(local_148 + 4) + -0x88;
        thunk_FUN_1400590e0(&local_140,2);
        thunk_FUN_140042b60(&local_148,"force agent termination");
        uVar4 = thunk_FUN_14007d360(&local_148,local_50);
        uVar5 = thunk_FUN_14018fec0(local_160);
        local_178 = 0x762;
        thunk_FUN_14018f150(uVar5,20000,uVar4,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-session\\sessionImpl.cpp"
                           );
        thunk_FUN_14018e980(local_160);
        if (0xf < local_38) {
          uVar6 = local_50[0];
          if (0xfff < local_38 + 1) {
            if ((local_50[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar6 = *(ulonglong *)(local_50[0] - 8);
            if (local_50[0] <= uVar6) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_50[0] - uVar6 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_50[0] - uVar6) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_14000219e(uVar6);
        }
        thunk_FUN_14005cd10(local_c0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_c0);
      }
      TerminateProcess(*(HANDLE *)(param_1 + 0x38),3);
    }
    CloseHandle(*(HANDLE *)(param_1 + 0x38));
    *(undefined8 *)(param_1 + 0x38) = 0xffffffffffffffff;
    *(undefined4 *)(param_1 + 0x40) = 0;
  }
  return 1;
}

// =============================================================================
// INFO
// =============================================================================

// -----------------------------------------------------------------------------
// Ghidra @ 0x14025b660
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_14025b660(longlong param_1,undefined8 *param_2)

{
  char cVar1;
  QDateTime *pQVar2;
  undefined8 uVar3;
  QString *this;
  QByteArray *this_00;
  char *pcVar4;
  undefined8 uVar5;
  ulonglong uVar6;
  undefined1 auStack_188 [32];
  undefined4 local_168;
  undefined4 local_158 [2];
  longlong *local_150;
  undefined8 uStack_148;
  QString local_140 [16];
  undefined8 local_130;
  undefined *local_128;
  basic_streambuf<char,std::char_traits<char>_> local_120 [128];
  basic_ios<char,std::char_traits<char>_> local_a0 [104];
  ulonglong local_38 [3];
  ulonglong local_20;
  ulonglong local_18;
  
  local_130 = 0xfffffffffffffffe;
  local_18 = DAT_1405169d0 ^ (ulonglong)auStack_188;
  local_158[0] = 0;
  local_150 = (longlong *)0x0;
  uStack_148 = 0;
  cVar1 = thunk_FUN_140273140(*param_2,&local_150);
  if ((cVar1 != '\0') && (local_150 != (longlong *)0x0)) {
    local_150[1] = *(longlong *)(param_1 + 0x1e8);
    pQVar2 = (QDateTime *)thunk_FUN_1402a8210(local_158);
    QDateTime::operator=((QDateTime *)(local_150 + 3),pQVar2);
    QDateTime::~QDateTime((QDateTime *)local_158);
    cVar1 = thunk_FUN_140267e30(local_150);
    if (cVar1 != '\0') {
      thunk_FUN_14025a9e0(param_1,&local_150);
      uVar3 = 1;
      goto LAB_14025b8d4;
    }
    uVar3 = thunk_FUN_14018fec0(local_140);
    cVar1 = thunk_FUN_14018fb20(uVar3,40000);
    thunk_FUN_14018e980(local_140);
    if (cVar1 != '\0') {
      local_128 = &DAT_140351f40;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_a0);
      local_158[0] = 1;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
      *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_130 + (longlong)*(int *)(local_128 + 4) + 4) =
           *(int *)(local_128 + 4) + -0x88;
      thunk_FUN_1400590e0(local_120,2);
      this = (QString *)(**(code **)(*local_150 + 0x18))(local_150,local_140);
      uVar3 = thunk_FUN_140042b60(&local_128,"invalid info message dropped before send: ");
      this_00 = (QByteArray *)QString::toLocal8Bit(this);
      pcVar4 = QByteArray::data(this_00);
      thunk_FUN_140042b60(uVar3,pcVar4);
      QByteArray::~QByteArray((QByteArray *)local_158);
      QString::~QString(local_140);
      uVar3 = thunk_FUN_14007d360(&local_128,local_38);
      uVar5 = thunk_FUN_14018fec0(local_140);
      local_168 = 0x130;
      thunk_FUN_14018f150(uVar5,40000,uVar3,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-info\\infoSink.cpp"
                         );
      thunk_FUN_14018e980(local_140);
      if (0xf < local_20) {
        uVar6 = local_38[0];
        if (0xfff < local_20 + 1) {
          if ((local_38[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar6 = *(ulonglong *)(local_38[0] - 8);
          if (local_38[0] <= uVar6) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_38[0] - uVar6 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_38[0] - uVar6) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_14000219e(uVar6);
      }
      thunk_FUN_14005cd10(local_a0);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_a0);
    }
  }
  uVar3 = 0;
LAB_14025b8d4:
  thunk_FUN_140260ef0(uStack_148);
  return uVar3;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14025b9b0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_14025b9b0(longlong param_1,undefined8 *param_2)

{
  longlong *plVar1;
  char cVar2;
  bool bVar3;
  QDateTime *pQVar4;
  undefined8 uVar5;
  QString *this;
  QByteArray *this_00;
  char *pcVar6;
  undefined8 uVar7;
  ulonglong uVar8;
  undefined1 auStack_188 [32];
  undefined4 local_168;
  undefined4 local_158 [2];
  longlong *local_150;
  undefined8 uStack_148;
  QString local_140 [16];
  undefined8 local_130;
  undefined *local_128;
  basic_streambuf<char,std::char_traits<char>_> local_120 [128];
  basic_ios<char,std::char_traits<char>_> local_a0 [104];
  ulonglong local_38 [3];
  ulonglong local_20;
  ulonglong local_18;
  
  local_130 = 0xfffffffffffffffe;
  local_18 = DAT_1405169d0 ^ (ulonglong)auStack_188;
  local_158[0] = 0;
  local_150 = (longlong *)0x0;
  uStack_148 = 0;
  cVar2 = thunk_FUN_140273970(*param_2,&local_150);
  if ((cVar2 != '\0') && (local_150 != (longlong *)0x0)) {
    local_150[1] = *(longlong *)(param_1 + 0x1e8);
    pQVar4 = (QDateTime *)thunk_FUN_1402a8210(local_158);
    QDateTime::operator=((QDateTime *)(local_150 + 3),pQVar4);
    QDateTime::~QDateTime((QDateTime *)local_158);
    plVar1 = local_150;
    if ((0 < local_150[1]) &&
       ((bVar3 = QString::isNull((QString *)(local_150 + 2)), !bVar3 &&
        (bVar3 = QDateTime::isValid((QDateTime *)(plVar1 + 3)), bVar3)))) {
      thunk_FUN_14025ac60(param_1,&local_150);
      uVar5 = 1;
      goto LAB_14025bc3b;
    }
    uVar5 = thunk_FUN_14018fec0(local_140);
    cVar2 = thunk_FUN_14018fb20(uVar5,40000);
    thunk_FUN_14018e980(local_140);
    if (cVar2 != '\0') {
      local_128 = &DAT_140351f40;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_a0);
      local_158[0] = 1;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
      *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_130 + (longlong)*(int *)(local_128 + 4) + 4) =
           *(int *)(local_128 + 4) + -0x88;
      thunk_FUN_1400590e0(local_120,2);
      this = (QString *)(**(code **)(*local_150 + 0x18))(local_150,local_140);
      uVar5 = thunk_FUN_140042b60(&local_128,"invalid info message dropped before send: ");
      this_00 = (QByteArray *)QString::toLocal8Bit(this);
      pcVar6 = QByteArray::data(this_00);
      thunk_FUN_140042b60(uVar5,pcVar6);
      QByteArray::~QByteArray((QByteArray *)local_158);
      QString::~QString(local_140);
      uVar5 = thunk_FUN_14007d360(&local_128,local_38);
      uVar7 = thunk_FUN_14018fec0(local_140);
      local_168 = 0x130;
      thunk_FUN_14018f150(uVar7,40000,uVar5,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-info\\infoSink.cpp"
                         );
      thunk_FUN_14018e980(local_140);
      if (0xf < local_20) {
        uVar8 = local_38[0];
        if (0xfff < local_20 + 1) {
          if ((local_38[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar8 = *(ulonglong *)(local_38[0] - 8);
          if (local_38[0] <= uVar8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_38[0] - uVar8 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_38[0] - uVar8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_14000219e(uVar8);
      }
      thunk_FUN_14005cd10(local_a0);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_a0);
    }
  }
  uVar5 = 0;
LAB_14025bc3b:
  thunk_FUN_140261090(uStack_148);
  return uVar5;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14025bd20
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_14025bd20(longlong param_1,undefined8 *param_2)

{
  longlong *plVar1;
  char cVar2;
  bool bVar3;
  QDateTime *pQVar4;
  undefined8 uVar5;
  QString *this;
  QByteArray *this_00;
  char *pcVar6;
  undefined8 uVar7;
  ulonglong uVar8;
  undefined1 auStack_188 [32];
  undefined4 local_168;
  undefined4 local_158 [2];
  longlong *local_150;
  undefined8 uStack_148;
  QString local_140 [16];
  undefined8 local_130;
  undefined *local_128;
  basic_streambuf<char,std::char_traits<char>_> local_120 [128];
  basic_ios<char,std::char_traits<char>_> local_a0 [104];
  ulonglong local_38 [3];
  ulonglong local_20;
  ulonglong local_18;
  
  local_130 = 0xfffffffffffffffe;
  local_18 = DAT_1405169d0 ^ (ulonglong)auStack_188;
  local_158[0] = 0;
  local_150 = (longlong *)0x0;
  uStack_148 = 0;
  cVar2 = thunk_FUN_140273a40(*param_2,&local_150);
  if ((cVar2 != '\0') && (local_150 != (longlong *)0x0)) {
    local_150[1] = *(longlong *)(param_1 + 0x1e8);
    pQVar4 = (QDateTime *)thunk_FUN_1402a8210(local_158);
    QDateTime::operator=((QDateTime *)(local_150 + 3),pQVar4);
    QDateTime::~QDateTime((QDateTime *)local_158);
    plVar1 = local_150;
    if ((0 < local_150[1]) &&
       ((bVar3 = QString::isNull((QString *)(local_150 + 2)), !bVar3 &&
        (bVar3 = QDateTime::isValid((QDateTime *)(plVar1 + 3)), bVar3)))) {
      thunk_FUN_14025ada0(param_1,&local_150);
      uVar5 = 1;
      goto LAB_14025bfab;
    }
    uVar5 = thunk_FUN_14018fec0(local_140);
    cVar2 = thunk_FUN_14018fb20(uVar5,40000);
    thunk_FUN_14018e980(local_140);
    if (cVar2 != '\0') {
      local_128 = &DAT_140351f40;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_a0);
      local_158[0] = 1;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
      *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_130 + (longlong)*(int *)(local_128 + 4) + 4) =
           *(int *)(local_128 + 4) + -0x88;
      thunk_FUN_1400590e0(local_120,2);
      this = (QString *)(**(code **)(*local_150 + 0x18))(local_150,local_140);
      uVar5 = thunk_FUN_140042b60(&local_128,"invalid info message dropped before send: ");
      this_00 = (QByteArray *)QString::toLocal8Bit(this);
      pcVar6 = QByteArray::data(this_00);
      thunk_FUN_140042b60(uVar5,pcVar6);
      QByteArray::~QByteArray((QByteArray *)local_158);
      QString::~QString(local_140);
      uVar5 = thunk_FUN_14007d360(&local_128,local_38);
      uVar7 = thunk_FUN_14018fec0(local_140);
      local_168 = 0x130;
      thunk_FUN_14018f150(uVar7,40000,uVar5,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-info\\infoSink.cpp"
                         );
      thunk_FUN_14018e980(local_140);
      if (0xf < local_20) {
        uVar8 = local_38[0];
        if (0xfff < local_20 + 1) {
          if ((local_38[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar8 = *(ulonglong *)(local_38[0] - 8);
          if (local_38[0] <= uVar8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_38[0] - uVar8 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_38[0] - uVar8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_14000219e(uVar8);
      }
      thunk_FUN_14005cd10(local_a0);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_a0);
    }
  }
  uVar5 = 0;
LAB_14025bfab:
  thunk_FUN_140261160(uStack_148);
  return uVar5;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14025c090
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_14025c090(longlong param_1,undefined8 *param_2)

{
  longlong *plVar1;
  char cVar2;
  bool bVar3;
  QDateTime *pQVar4;
  undefined8 uVar5;
  QString *this;
  QByteArray *this_00;
  char *pcVar6;
  undefined8 uVar7;
  ulonglong uVar8;
  undefined1 auStack_188 [32];
  undefined4 local_168;
  undefined4 local_158 [2];
  longlong *local_150;
  undefined8 uStack_148;
  QString local_140 [16];
  undefined8 local_130;
  undefined *local_128;
  basic_streambuf<char,std::char_traits<char>_> local_120 [128];
  basic_ios<char,std::char_traits<char>_> local_a0 [104];
  ulonglong local_38 [3];
  ulonglong local_20;
  ulonglong local_18;
  
  local_130 = 0xfffffffffffffffe;
  local_18 = DAT_1405169d0 ^ (ulonglong)auStack_188;
  local_158[0] = 0;
  local_150 = (longlong *)0x0;
  uStack_148 = 0;
  cVar2 = thunk_FUN_140273b10(*param_2,&local_150);
  if ((cVar2 != '\0') && (local_150 != (longlong *)0x0)) {
    local_150[1] = *(longlong *)(param_1 + 0x1e8);
    pQVar4 = (QDateTime *)thunk_FUN_1402a8210(local_158);
    QDateTime::operator=((QDateTime *)(local_150 + 3),pQVar4);
    QDateTime::~QDateTime((QDateTime *)local_158);
    plVar1 = local_150;
    if ((0 < local_150[1]) &&
       ((bVar3 = QString::isNull((QString *)(local_150 + 2)), !bVar3 &&
        (bVar3 = QDateTime::isValid((QDateTime *)(plVar1 + 3)), bVar3)))) {
      thunk_FUN_14025aee0(param_1,&local_150);
      uVar5 = 1;
      goto LAB_14025c31b;
    }
    uVar5 = thunk_FUN_14018fec0(local_140);
    cVar2 = thunk_FUN_14018fb20(uVar5,40000);
    thunk_FUN_14018e980(local_140);
    if (cVar2 != '\0') {
      local_128 = &DAT_140351f40;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_a0);
      local_158[0] = 1;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
      *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_130 + (longlong)*(int *)(local_128 + 4) + 4) =
           *(int *)(local_128 + 4) + -0x88;
      thunk_FUN_1400590e0(local_120,2);
      this = (QString *)(**(code **)(*local_150 + 0x18))(local_150,local_140);
      uVar5 = thunk_FUN_140042b60(&local_128,"invalid info message dropped before send: ");
      this_00 = (QByteArray *)QString::toLocal8Bit(this);
      pcVar6 = QByteArray::data(this_00);
      thunk_FUN_140042b60(uVar5,pcVar6);
      QByteArray::~QByteArray((QByteArray *)local_158);
      QString::~QString(local_140);
      uVar5 = thunk_FUN_14007d360(&local_128,local_38);
      uVar7 = thunk_FUN_14018fec0(local_140);
      local_168 = 0x130;
      thunk_FUN_14018f150(uVar7,40000,uVar5,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-info\\infoSink.cpp"
                         );
      thunk_FUN_14018e980(local_140);
      if (0xf < local_20) {
        uVar8 = local_38[0];
        if (0xfff < local_20 + 1) {
          if ((local_38[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar8 = *(ulonglong *)(local_38[0] - 8);
          if (local_38[0] <= uVar8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_38[0] - uVar8 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_38[0] - uVar8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_14000219e(uVar8);
      }
      thunk_FUN_14005cd10(local_a0);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_a0);
    }
  }
  uVar5 = 0;
LAB_14025c31b:
  thunk_FUN_140261230(uStack_148);
  return uVar5;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14025caa0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_14025caa0(longlong param_1,undefined8 *param_2)

{
  char cVar1;
  QDateTime *pQVar2;
  undefined8 uVar3;
  QString *this;
  QByteArray *this_00;
  char *pcVar4;
  undefined8 uVar5;
  ulonglong uVar6;
  undefined1 auStack_188 [32];
  undefined4 local_168;
  undefined4 local_158 [2];
  longlong *local_150;
  undefined8 uStack_148;
  QString local_140 [16];
  undefined8 local_130;
  undefined *local_128;
  basic_streambuf<char,std::char_traits<char>_> local_120 [128];
  basic_ios<char,std::char_traits<char>_> local_a0 [104];
  ulonglong local_38 [3];
  ulonglong local_20;
  ulonglong local_18;
  
  local_130 = 0xfffffffffffffffe;
  local_18 = DAT_1405169d0 ^ (ulonglong)auStack_188;
  local_158[0] = 0;
  local_150 = (longlong *)0x0;
  uStack_148 = 0;
  cVar1 = thunk_FUN_140273f40(*param_2,&local_150);
  if ((cVar1 != '\0') && (local_150 != (longlong *)0x0)) {
    local_150[1] = *(longlong *)(param_1 + 0x1e8);
    pQVar2 = (QDateTime *)thunk_FUN_1402a8210(local_158);
    QDateTime::operator=((QDateTime *)(local_150 + 3),pQVar2);
    QDateTime::~QDateTime((QDateTime *)local_158);
    cVar1 = thunk_FUN_140268210(local_150);
    if (cVar1 != '\0') {
      thunk_FUN_14025b3e0(param_1,&local_150);
      uVar3 = 1;
      goto LAB_14025cd14;
    }
    uVar3 = thunk_FUN_14018fec0(local_140);
    cVar1 = thunk_FUN_14018fb20(uVar3,40000);
    thunk_FUN_14018e980(local_140);
    if (cVar1 != '\0') {
      local_128 = &DAT_140351f40;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_a0);
      local_158[0] = 1;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
      *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_130 + (longlong)*(int *)(local_128 + 4) + 4) =
           *(int *)(local_128 + 4) + -0x88;
      thunk_FUN_1400590e0(local_120,2);
      this = (QString *)(**(code **)(*local_150 + 0x18))(local_150,local_140);
      uVar3 = thunk_FUN_140042b60(&local_128,"invalid info message dropped before send: ");
      this_00 = (QByteArray *)QString::toLocal8Bit(this);
      pcVar4 = QByteArray::data(this_00);
      thunk_FUN_140042b60(uVar3,pcVar4);
      QByteArray::~QByteArray((QByteArray *)local_158);
      QString::~QString(local_140);
      uVar3 = thunk_FUN_14007d360(&local_128,local_38);
      uVar5 = thunk_FUN_14018fec0(local_140);
      local_168 = 0x130;
      thunk_FUN_14018f150(uVar5,40000,uVar3,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-info\\infoSink.cpp"
                         );
      thunk_FUN_14018e980(local_140);
      if (0xf < local_20) {
        uVar6 = local_38[0];
        if (0xfff < local_20 + 1) {
          if ((local_38[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar6 = *(ulonglong *)(local_38[0] - 8);
          if (local_38[0] <= uVar6) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_38[0] - uVar6 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_38[0] - uVar6) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_14000219e(uVar6);
      }
      thunk_FUN_14005cd10(local_a0);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_a0);
    }
  }
  uVar3 = 0;
LAB_14025cd14:
  thunk_FUN_140261570(uStack_148);
  return uVar3;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14025cdf0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_14025cdf0(longlong param_1,undefined8 *param_2)

{
  char cVar1;
  QDateTime *pQVar2;
  undefined8 uVar3;
  QString *this;
  QByteArray *this_00;
  char *pcVar4;
  undefined8 uVar5;
  ulonglong uVar6;
  undefined1 auStack_188 [32];
  undefined4 local_168;
  undefined4 local_158 [2];
  longlong *local_150;
  undefined8 uStack_148;
  QString local_140 [16];
  undefined8 local_130;
  undefined *local_128;
  basic_streambuf<char,std::char_traits<char>_> local_120 [128];
  basic_ios<char,std::char_traits<char>_> local_a0 [104];
  ulonglong local_38 [3];
  ulonglong local_20;
  ulonglong local_18;
  
  local_130 = 0xfffffffffffffffe;
  local_18 = DAT_1405169d0 ^ (ulonglong)auStack_188;
  local_158[0] = 0;
  local_150 = (longlong *)0x0;
  uStack_148 = 0;
  cVar1 = thunk_FUN_1402742f0(*param_2,&local_150);
  if ((cVar1 != '\0') && (local_150 != (longlong *)0x0)) {
    local_150[1] = *(longlong *)(param_1 + 0x1e8);
    pQVar2 = (QDateTime *)thunk_FUN_1402a8210(local_158);
    QDateTime::operator=((QDateTime *)(local_150 + 3),pQVar2);
    QDateTime::~QDateTime((QDateTime *)local_158);
    cVar1 = thunk_FUN_140268330(local_150);
    if (cVar1 != '\0') {
      thunk_FUN_14025b520(param_1,&local_150);
      uVar3 = 1;
      goto LAB_14025d064;
    }
    uVar3 = thunk_FUN_14018fec0(local_140);
    cVar1 = thunk_FUN_14018fb20(uVar3,40000);
    thunk_FUN_14018e980(local_140);
    if (cVar1 != '\0') {
      local_128 = &DAT_140351f40;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_a0);
      local_158[0] = 1;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
      *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_130 + (longlong)*(int *)(local_128 + 4) + 4) =
           *(int *)(local_128 + 4) + -0x88;
      thunk_FUN_1400590e0(local_120,2);
      this = (QString *)(**(code **)(*local_150 + 0x18))(local_150,local_140);
      uVar3 = thunk_FUN_140042b60(&local_128,"invalid info message dropped before send: ");
      this_00 = (QByteArray *)QString::toLocal8Bit(this);
      pcVar4 = QByteArray::data(this_00);
      thunk_FUN_140042b60(uVar3,pcVar4);
      QByteArray::~QByteArray((QByteArray *)local_158);
      QString::~QString(local_140);
      uVar3 = thunk_FUN_14007d360(&local_128,local_38);
      uVar5 = thunk_FUN_14018fec0(local_140);
      local_168 = 0x130;
      thunk_FUN_14018f150(uVar5,40000,uVar3,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-info\\infoSink.cpp"
                         );
      thunk_FUN_14018e980(local_140);
      if (0xf < local_20) {
        uVar6 = local_38[0];
        if (0xfff < local_20 + 1) {
          if ((local_38[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar6 = *(ulonglong *)(local_38[0] - 8);
          if (local_38[0] <= uVar6) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_38[0] - uVar6 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_38[0] - uVar6) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_14000219e(uVar6);
      }
      thunk_FUN_14005cd10(local_a0);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_a0);
    }
  }
  uVar3 = 0;
LAB_14025d064:
  thunk_FUN_140261640(uStack_148);
  return uVar3;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140263040
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140263040(QObject *param_1,undefined8 *param_2)

{
  ulonglong uVar1;
  ulonglong uVar2;
  char cVar3;
  undefined8 uVar4;
  longlong lVar5;
  QString *pQVar6;
  QByteArray *this;
  char *pcVar7;
  undefined8 uVar8;
  char *pcVar9;
  ulonglong *puVar10;
  QDateTime *pQVar11;
  undefined ***pppuVar12;
  undefined1 auStack_1a8 [32];
  undefined4 local_188;
  ulonglong local_178;
  ulonglong uStack_170;
  QObject *local_168;
  undefined8 local_160;
  undefined8 local_158;
  undefined8 local_150;
  undefined *local_148;
  undefined **local_140;
  basic_ostream<char,std::char_traits<char>_> local_138 [96];
  undefined8 local_d8;
  undefined4 local_d0;
  basic_ios<char,std::char_traits<char>_> local_c0 [104];
  undefined ***local_58;
  undefined8 local_50;
  ulonglong local_40;
  ulonglong local_38;
  
  local_158 = 0xfffffffffffffffe;
  local_38 = DAT_1405169d0 ^ (ulonglong)auStack_1a8;
  local_178 = local_178 & 0xffffffff00000000;
  local_150 = param_2;
  thunk_FUN_14012de90(param_1 + 0x1d8);
  uVar4 = (**(code **)(*(longlong *)*param_2 + 0x60))((longlong *)*param_2,&local_178);
  lVar5 = thunk_FUN_140261ee0(*(undefined8 *)(param_1 + 0x1f0),uVar4);
  QString::~QString((QString *)&local_178);
  if (lVar5 == 0) {
    uVar4 = thunk_FUN_14018fec0(&local_168);
    cVar3 = thunk_FUN_14018fb20(uVar4,30000);
    thunk_FUN_14018e980(&local_168);
    if (cVar3 != '\0') {
      local_148 = &DAT_140351f40;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_c0);
      local_178 = CONCAT44(local_178._4_4_,1);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_148,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_140,false);
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_150 + (longlong)*(int *)(local_148 + 4) + 4) =
           *(int *)(local_148 + 4) + -0x88;
      local_58 = &local_140;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_d8 = 0;
      local_d0 = 4;
      pQVar6 = (QString *)
               (**(code **)(*(longlong *)*param_2 + 0x60))((longlong *)*param_2,&local_58);
      uVar4 = thunk_FUN_140042b60(&local_148,"info: session shutdown for unknown sequencer ");
      this = (QByteArray *)QString::toLocal8Bit(pQVar6);
      pcVar7 = QByteArray::data(this);
      thunk_FUN_140042b60(uVar4,pcVar7);
      QByteArray::~QByteArray((QByteArray *)&local_178);
      QString::~QString((QString *)&local_58);
      uVar4 = thunk_FUN_14007d360(&local_148,&local_58);
      uVar8 = thunk_FUN_14018fec0(&local_168);
      local_188 = 0xbb;
      thunk_FUN_14018f150(uVar8,30000,uVar4,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-info\\infoSink.cpp"
                         );
      thunk_FUN_14018e980(&local_168);
      if (0xf < local_40) {
        pppuVar12 = local_58;
        if (0xfff < local_40 + 1) {
          if (((ulonglong)local_58 & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          pppuVar12 = (undefined ***)local_58[-1];
          if (local_58 <= pppuVar12) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if ((ulonglong)((longlong)local_58 - (longlong)pppuVar12) < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < (ulonglong)((longlong)local_58 - (longlong)pppuVar12)) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_14000219e(pppuVar12);
      }
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_150 + (longlong)*(int *)(local_148 + 4) + 4) =
           *(int *)(local_148 + 4) + -0x88;
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140065d80(&local_140);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_138);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_c0);
    }
  }
  else {
    local_178 = 0;
    uStack_170 = 0;
    uVar4 = (**(code **)(*(longlong *)*param_2 + 0x60))((longlong *)*param_2,&local_58);
    thunk_FUN_1402683a0(param_1 + 0x1f0,&local_168,uVar4,&local_178);
    QString::~QString((QString *)&local_58);
    thunk_FUN_140261710(uStack_170);
    if (local_168 != (QObject *)0x0) {
      pcVar7 = qFlagLocation("1sequencerWantFlushContinuous(QString)");
      pcVar9 = qFlagLocation("2wantFlushContinuous(QString)");
      QObject::disconnect(local_168,pcVar9,param_1,pcVar7);
    }
    uVar4 = (**(code **)(*(longlong *)*param_2 + 0x60))((longlong *)*param_2,&local_178);
    thunk_FUN_140266800(param_1 + 0x1f0,uVar4);
    QString::~QString((QString *)&local_178);
    uVar4 = (**(code **)(*(longlong *)*param_2 + 0x60))((longlong *)*param_2,&local_178);
    thunk_FUN_140266800(param_1 + 0x1f8,uVar4);
    QString::~QString((QString *)&local_178);
    local_178 = 0;
    uStack_170 = 0;
    puVar10 = (ulonglong *)thunk_FUN_140257c80(&local_58);
    uVar1 = *puVar10;
    uVar2 = puVar10[1];
    puVar10[1] = 0;
    *puVar10 = 0;
    local_178 = uVar1;
    uStack_170 = uVar2;
    thunk_FUN_140261640(0);
    thunk_FUN_140261640(local_50);
    *(undefined8 *)(uVar1 + 8) = *(undefined8 *)(param_1 + 0x1e8);
    pQVar6 = (QString *)(**(code **)(*(longlong *)*param_2 + 0x60))((longlong *)*param_2,&local_58);
    QString::operator=((QString *)(uVar1 + 0x10),pQVar6);
    QString::~QString((QString *)&local_58);
    pQVar11 = (QDateTime *)thunk_FUN_1402a8210(&local_58);
    QDateTime::operator=((QDateTime *)(uVar1 + 0x18),pQVar11);
    QDateTime::~QDateTime((QDateTime *)&local_58);
    *(undefined4 *)(uVar1 + 0x20) = 0;
    pQVar11 = (QDateTime *)
              (**(code **)(*(longlong *)*param_2 + 0x80))((longlong *)*param_2,&local_58);
    QDateTime::operator=((QDateTime *)(uVar1 + 0x28),pQVar11);
    QDateTime::~QDateTime((QDateTime *)&local_58);
    pQVar6 = (QString *)(**(code **)(*(longlong *)*param_2 + 0x78))((longlong *)*param_2,&local_58);
    QString::operator=((QString *)(uVar1 + 0x30),pQVar6);
    QString::~QString((QString *)&local_58);
    pQVar6 = (QString *)(**(code **)(*(longlong *)*param_2 + 0x68))((longlong *)*param_2,&local_58);
    QString::operator=((QString *)(uVar1 + 0x38),pQVar6);
    QString::~QString((QString *)&local_58);
    pQVar6 = (QString *)(**(code **)(*(longlong *)*param_2 + 0x70))((longlong *)*param_2,&local_58);
    QString::operator=((QString *)(uVar1 + 0x40),pQVar6);
    QString::~QString((QString *)&local_58);
    thunk_FUN_14025b520(param_1,&local_178);
    thunk_FUN_140261640(uVar2);
    thunk_FUN_140261710(local_160);
  }
  thunk_FUN_1400e5920(param_2[1]);
  return;
}

// =============================================================================
// VIDEO
// =============================================================================

// -----------------------------------------------------------------------------
// Ghidra @ 0x140286f70
// -----------------------------------------------------------------------------
undefined8 *
initialize_video_router
          (undefined8 *param_1,undefined8 *param_2,undefined8 param_3,undefined8 param_4)

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
  thunk_FUN_1402864c0(param_1,&local_res18,&local_98,&local_78);
  QString::~QString((QString *)&local_res18);
  QString::~QString((QString *)&local_98);
  *param_1 = grabber::VideoSink::vftable;
  param_1[0x39] = 0;
  param_1[0x3a] = param_3;
  param_1[0x3c] = shared_null_exref;
  QTimer::QTimer((QTimer *)(param_1 + 0x3d),(QObject *)0x0);
  param_1[0x41] = shared_null_exref;
  param_1[0x42] = shared_null_exref;
  param_1[0x43] = shared_null_exref;
  param_1[0x44] = shared_null_exref;
  param_1[0x45] = shared_null_exref;
  uVar1 = thunk_FUN_140076bb0();
  lVar2 = thunk_FUN_1400a5120(uVar1,local_48);
  param_1[0x39] = *(undefined8 *)(lVar2 + 8);
  thunk_FUN_140291d40(param_1,param_4);
  *(uint *)((longlong)param_1 + 0x204) = *(uint *)((longlong)param_1 + 0x204) & 0xfffffffe;
  local_78 = thunk_FUN_14028ddb0;
  uStack_5c = (undefined4)((ulonglong)piStack_70 >> 0x20);
  piStack_70 = (int *)((ulonglong)piStack_70 & 0xffffffff00000000);
  local_68 = 0x40013caf;
  uStack_64 = 1;
  uStack_60 = 0;
  thunk_FUN_140283f90(local_90,param_1 + 0x3d,timeout_exref,param_1,&local_68,0);
  QMetaObject::Connection::~Connection(local_90);
  local_78 = (code *)&LAB_14000a218;
  uStack_4c = (undefined4)((ulonglong)piStack_70 >> 0x20);
  piStack_70 = (int *)((ulonglong)piStack_70 & 0xffffffff00000000);
  local_58 = 0x4000a218;
  uStack_54 = 1;
  uStack_50 = 0;
  thunk_FUN_140284080(local_88,param_1[0x34],thunk_FUN_1401f6690,param_1,&local_58,0);
  QMetaObject::Connection::~Connection(local_88);
  thunk_FUN_1400e5ac0(param_2[1]);
  return param_1;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14028e4a0
// -----------------------------------------------------------------------------
void FUN_14028e4a0(longlong param_1,QString *param_2,longlong *param_3)

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
              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-video\\videoSink.cpp",0x124
             );
  }
  thunk_FUN_1402a8210(local_res8);
  plVar1 = (longlong *)(param_1 + 0x1e0);
  if (1 < *(uint *)*plVar1) {
    thunk_FUN_14028b9f0(plVar1);
  }
  lVar2 = *plVar1;
  if (*(longlong *)(lVar2 + 0x10) == 0) {
    this = (QMapNodeBase *)(lVar2 + 8);
  }
  else {
    this = *(QMapNodeBase **)(lVar2 + 0x20);
  }
  if (1 < *(uint *)*plVar1) {
    thunk_FUN_14028b9f0(plVar1);
  }
  lVar2 = *plVar1;
  for (; this != (QMapNodeBase *)(lVar2 + 8); this = QMapNodeBase::nextNode(this)) {
    bVar3 = operator==((QString *)(this + 0x20),param_2);
    if ((((bVar3) && (*(int *)(this + 0x30) == 2 - (uint)(((QString *)*param_3)[8] != (QString)0x0))
         ) && (bVar3 = operator==((QString *)(this + 0x28),(QString *)*param_3), bVar3)) &&
       (*(int *)(this + 0x34) == *(int *)(*param_3 + 0xc))) {
      thunk_FUN_1402939b0(*(undefined8 *)(this + 0x50),local_res8);
      thunk_FUN_1402912b0(param_1,local_res8,this + 0x18,this + 0x40);
    }
  }
  QDateTime::~QDateTime(local_res8);
  QString::~QString(param_2);
  thunk_FUN_1400e5370(param_3[1]);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14028fa10
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14028fa10(longlong param_1,longlong *param_2)

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
  local_30 = DAT_1405169d0 ^ (ulonglong)auStack_198;
  local_158[0] = 0;
  lVar7 = *param_2;
  if (*(longlong *)(lVar7 + 8) == *(longlong *)(param_1 + 0x1c8)) {
    plVar1 = (longlong *)(param_1 + 0x1e0);
    if (1 < *(uint *)*plVar1) {
      thunk_FUN_14028b9f0(plVar1);
    }
    lVar7 = thunk_FUN_14028c830(*plVar1,lVar7 + 8);
    if (lVar7 == 0) {
      lVar7 = *plVar1 + 8;
    }
    if (1 < *(uint *)*plVar1) {
      thunk_FUN_14028b9f0(plVar1);
    }
    if (*plVar1 + 8 == lVar7) {
      uVar3 = thunk_FUN_14018fec0(local_168);
      cVar2 = thunk_FUN_14018fb20(uVar3,30000);
      thunk_FUN_14018e980(local_168);
      if (cVar2 != '\0') {
        local_148 = &DAT_140351f40;
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
        thunk_FUN_1400590e0(&local_140,2);
        this = (QString *)thunk_FUN_140081420(*param_2 + 8,local_168);
        uVar3 = thunk_FUN_140042b60(&local_148,"absent stream frame response effort: ");
        this_00 = (QByteArray *)QString::toLocal8Bit(this);
        pcVar8 = QByteArray::data(this_00);
        thunk_FUN_140042b60(uVar3,pcVar8);
        QByteArray::~QByteArray((QByteArray *)local_158);
        QString::~QString((QString *)local_168);
        uVar3 = thunk_FUN_14007d360(&local_148,local_50);
        uVar6 = thunk_FUN_14018fec0(local_168);
        local_178 = 0x8a;
        thunk_FUN_14018f150(uVar6,30000,uVar3,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-video\\videoSink.cpp"
                           );
        thunk_FUN_14018e980(local_168);
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
          FUN_14000219e(uVar9);
        }
        thunk_FUN_14005cd10(local_c0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_c0);
      }
    }
    else {
      if (*(longlong *)(lVar7 + 0x50) != 0) {
        thunk_FUN_140295c30(*(longlong *)(lVar7 + 0x50),*param_2 + 0x30);
      }
      uVar3 = thunk_FUN_1402a8210(local_168);
      thunk_FUN_1402912b0(param_1,uVar3,*param_2 + 8,lVar7 + 0x40);
      QDateTime::~QDateTime((QDateTime *)local_168);
    }
  }
  else {
    uVar3 = thunk_FUN_14018fec0(local_168);
    cVar2 = thunk_FUN_14018fb20(uVar3,30000);
    thunk_FUN_14018e980(local_168);
    if (cVar2 != '\0') {
      local_148 = &DAT_140351f40;
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
               thunk_FUN_140042b60(&local_148,"alien grabber stream frame response effort, this ");
      pbVar5 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                         (pbVar4,*(__int64 *)(param_1 + 0x1c8));
      pbVar4 = (basic_ostream<char,std::char_traits<char>_> *)thunk_FUN_140042b60(pbVar5,", try ");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar4,*(__int64 *)(lVar7 + 8));
      uVar3 = thunk_FUN_14007d360(&local_148,local_50);
      uVar6 = thunk_FUN_14018fec0(local_168);
      local_178 = 0x82;
      thunk_FUN_14018f150(uVar6,30000,uVar3,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-video\\videoSink.cpp"
                         );
      thunk_FUN_14018e980(local_168);
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
        FUN_14000219e(uVar9);
      }
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140065d80(&local_140);
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

// -----------------------------------------------------------------------------
// Ghidra @ 0x140290000
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140290000(longlong param_1,longlong *param_2)

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
  local_78 = DAT_1405169d0 ^ (ulonglong)auStack_398;
  uVar25 = 0;
  local_368 = 0;
  iVar28 = 0;
  local_360 = param_1;
  local_350 = param_2;
  iVar10 = QListData::size((QListData *)(*param_2 + 8));
  dVar5 = DAT_140450848;
  dVar4 = DAT_140434520;
  if (0 < iVar10) {
    do {
      pQVar23 = (QListData *)(*param_2 + 8);
      if ((iVar28 < 0) || (iVar10 = QListData::size(pQVar23), iVar10 <= iVar28)) {
        qt_assert_x("QList<T>::operator[]","index out of range",
                    "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",0x21b);
      }
      if (1 < **(uint **)pQVar23) {
        thunk_FUN_14006fc40(pQVar23,(*(uint **)pQVar23)[1]);
      }
      ppvVar11 = QListData::at(pQVar23,iVar28);
      plVar3 = *ppvVar11;
      if (*plVar3 == *(longlong *)(param_1 + 0x1c8)) {
        plVar1 = (longlong *)(param_1 + 0x208);
        if (1 < *(uint *)*plVar1) {
          thunk_FUN_1400faf20(plVar1);
        }
        if (*(longlong *)(*plVar1 + 0x10) == 0) {
LAB_14029033c:
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
          goto LAB_14029033c;
        }
        if (1 < *(uint *)*plVar1) {
          thunk_FUN_1400faf20(plVar1);
        }
        if (*plVar1 + 8 == lVar27) {
          uVar12 = thunk_FUN_14018fec0(local_2b8);
          cVar7 = thunk_FUN_14018fb20(uVar12,30000);
          thunk_FUN_14018e980(local_2b8);
          param_1 = local_360;
          if (cVar7 != '\0') {
            local_1a8 = &DAT_140351f40;
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
            uVar12 = thunk_FUN_140042b60(&local_1a8,"alien session stream selection effort, ");
            this = (QByteArray *)QString::toLocal8Bit((QString *)(plVar3 + 1));
            pcVar16 = QByteArray::data(this);
            thunk_FUN_140042b60(uVar12,pcVar16);
            QByteArray::~QByteArray(local_310);
            uVar12 = thunk_FUN_14007d360(&local_1a8,local_98);
            uVar15 = thunk_FUN_14018fec0(local_2a8);
            local_378 = 0x32;
            thunk_FUN_14018f150(uVar15,30000,uVar12,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-video\\videoSink.cpp"
                               );
            thunk_FUN_14018e980(local_2a8);
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
              FUN_14000219e(uVar22);
            }
            *(undefined ***)((longlong)&local_1a8 + (longlong)*(int *)(local_1a8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1ac + (longlong)*(int *)(local_1a8 + 4)) =
                 *(int *)(local_1a8 + 4) + -0x88;
            local_1a0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                        vftable;
            thunk_FUN_140065d80(&local_1a0);
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
          pppuVar26 = (undefined ***)(local_360 + 0x1e0);
          local_348 = pppuVar26;
          puVar17 = (undefined8 *)thunk_FUN_1402883d0(pppuVar26,plVar3);
          if (puVar17[2] == 0) {
            local_2e8 = operator_new(0xb8);
            if (local_2e8 == (void *)0x0) {
              lVar18 = 0;
            }
            else {
              lVar18 = thunk_FUN_140293480(local_2e8,*(undefined4 *)((longlong)plVar3 + 0x1c));
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
                *(undefined1 **)(puVar19 + 2) = &LAB_140009435;
                puVar19[1] = 1;
                *puVar19 = 1;
              }
              local_358 = puVar19;
              local_2f8 = puVar19;
              thunk_FUN_14007cde0(puVar19,lVar18,1);
              FUN_140002059(local_300,lVar18);
            }
            local_358 = (undefined4 *)puVar17[3];
            puVar17[3] = puVar19;
            puVar17[2] = lVar18;
            thunk_FUN_14028b290();
            thunk_FUN_14028b290(0);
            puVar17[4] = 0;
            puVar20 = (undefined8 *)thunk_FUN_140288530(plVar1,plVar3 + 1);
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
            thunk_FUN_1400e5920();
            pppuVar26 = local_348;
          }
          pQVar23 = (QListData *)(*local_350 + 0x10);
          if ((iVar28 < 0) || (iVar10 = QListData::size(pQVar23), iVar10 <= iVar28)) {
            qt_assert_x("QList<T>::operator[]","index out of range",
                        "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",0x21b
                       );
          }
          if (1 < **(uint **)pQVar23) {
            thunk_FUN_14006fb80(pQVar23,(*(uint **)pQVar23)[1]);
          }
          ppvVar11 = QListData::at(pQVar23,iVar28);
          plVar6 = local_350;
          if ((double)*ppvVar11 <= 0.0) {
            if ((*(int *)((longlong)plVar3 + 0x1c) == -1) && ((int)plVar3[3] == 1)) {
              puVar17[4] = 0;
              thunk_FUN_140294580(puVar17[2]);
            }
            else {
              thunk_FUN_140290c10(pppuVar26,plVar3);
            }
            QString::QString(local_340);
            local_338 = (int)plVar3[3] == 1;
            QString::operator=(local_340,(QString *)(plVar3 + 2));
            local_334 = *(undefined4 *)((longlong)plVar3 + 0x1c);
            puVar17 = (undefined8 *)thunk_FUN_140288530(plVar1,plVar3 + 1);
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
              thunk_FUN_14006fb80(pQVar23,(*(uint **)pQVar23)[1]);
            }
            ppvVar11 = QListData::at(pQVar23,iVar28);
            if ((double)puVar17[4] != (double)*ppvVar11) {
              pdVar21 = (double *)thunk_FUN_140288250(*plVar6 + 0x10,iVar28);
              dVar2 = *pdVar21;
              puVar17[4] = dVar2;
              if ((char)plVar3[4] != '\0') {
                thunk_FUN_140296a70(puVar17[2],(int)(dVar2 * dVar4 + dVar5));
                uVar12 = puVar17[2];
                uVar9 = thunk_FUN_140068580(local_360 + 0x228,plVar3 + 1);
                thunk_FUN_140296b70(uVar12,uVar9);
              }
            }
            uVar12 = thunk_FUN_1402a8210(local_308);
            param_1 = local_360;
            thunk_FUN_1402912b0(local_360,uVar12,plVar3,puVar17);
            QDateTime::~QDateTime(local_308);
            uVar25 = local_368;
          }
        }
      }
      else {
        uVar12 = thunk_FUN_14018fec0(local_2d8);
        cVar7 = thunk_FUN_14018fb20(uVar12,30000);
        thunk_FUN_14018e980(local_2d8);
        if (cVar7 != '\0') {
          local_298 = &DAT_140351f40;
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
                    thunk_FUN_140042b60((basic_ostream<char,std::char_traits<char>_> *)&local_298,
                                        "alien grabber stream selection effort, this ");
          pbVar14 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                              (pbVar13,*(__int64 *)(param_1 + 0x1c8));
          pbVar13 = (basic_ostream<char,std::char_traits<char>_> *)
                    thunk_FUN_140042b60(pbVar14,", try ");
          std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar13,*plVar3);
          uVar12 = thunk_FUN_14007d360((basic_ostream<char,std::char_traits<char>_> *)&local_298,
                                       local_b8);
          uVar15 = thunk_FUN_14018fec0(local_2c8);
          local_378 = 0x2b;
          thunk_FUN_14018f150(uVar15,30000,uVar12,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-video\\videoSink.cpp"
                             );
          thunk_FUN_14018e980(local_2c8);
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
            FUN_14000219e(uVar22);
          }
          *(undefined ***)
           ((basic_ostream<char,std::char_traits<char>_> *)&local_298 + *(int *)(local_298 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_29c + (longlong)*(int *)(local_298 + 4)) =
               *(int *)(local_298 + 4) + -0x88;
          local_290 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                      vftable;
          thunk_FUN_140065d80(&local_290);
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
  thunk_FUN_140292890(param_1);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1402912b0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1402912b0(longlong param_1,undefined8 param_2,longlong param_3,longlong param_4)

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
  local_38 = DAT_1405169d0 ^ (ulonglong)auStack_1c8;
  local_190[0] = 0;
  if (((0.0 < *(double *)(param_4 + 0x20)) && (*(longlong *)(param_4 + 0x10) != 0)) &&
     (cVar1 = thunk_FUN_140295ed0(), cVar1 != '\0')) {
    build_video_delta_packets(*(undefined8 *)(param_4 + 0x10),&local_188);
    if (local_188 != 0) {
      uVar3 = QDateTime::QDateTime(local_168);
      thunk_FUN_140292fb0(param_1 + 0x210,local_198,param_3 + 8,uVar3);
      QDateTime::~QDateTime(local_168);
      bVar2 = QDateTime::isValid(local_198);
      if ((!bVar2) ||
         (bVar2 = QDateTime::operator<((QDateTime *)(local_188 + 0x30),local_198), bVar2)) {
        uVar3 = thunk_FUN_14018fec0(local_178);
        cVar1 = thunk_FUN_14018fb20(uVar3,30000);
        thunk_FUN_14018e980(local_178);
        if (cVar1 != '\0') {
          local_158 = &DAT_140351f40;
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
          thunk_FUN_1400590e0(local_150,2);
          this = (QString *)thunk_FUN_140081420(param_3,local_178);
          uVar3 = thunk_FUN_140042b60(&local_158,
                                      "trying to send frame while time is not negotiated: ");
          uVar3 = thunk_FUN_1400a18b0(uVar3,local_198);
          uVar3 = thunk_FUN_140042b60(uVar3,&DAT_140352584);
          uVar3 = thunk_FUN_1400a18b0(uVar3,local_188 + 0x30);
          uVar3 = thunk_FUN_140042b60(uVar3," for ");
          this_00 = (QByteArray *)QString::toLocal8Bit(this);
          pcVar4 = QByteArray::data(this_00);
          thunk_FUN_140042b60(uVar3,pcVar4);
          QByteArray::~QByteArray((QByteArray *)local_190);
          QString::~QString(local_178);
          uVar3 = thunk_FUN_14007d360(&local_158,&local_68);
          uVar5 = thunk_FUN_14018fec0(local_178);
          local_1a8 = 0x1de;
          thunk_FUN_14018f150(uVar5,30000,uVar3,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-video\\videoSink.cpp"
                             );
          thunk_FUN_14018e980(local_178);
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
            FUN_14000219e(uVar6);
          }
          thunk_FUN_14005cd10(local_d0);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_d0);
        }
        thunk_FUN_140294580(*(undefined8 *)(param_4 + 0x10));
      }
      else {
        uVar3 = thunk_FUN_140284e70(&local_68,param_3,local_188 + 0x30);
        thunk_FUN_14028cf40(param_1 + 0x220,local_178,param_3 + 0x10,uVar3);
        QDateTime::~QDateTime(local_40);
        QString::~QString(local_58);
        QString::~QString(local_60);
        thunk_FUN_140287eb0(local_188 + 8,param_3);
        thunk_FUN_140285920(param_1,&local_188);
      }
      QDateTime::~QDateTime(local_198);
    }
    thunk_FUN_14028b1c0(local_180);
  }
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140293ba0
// -----------------------------------------------------------------------------
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 * build_video_delta_packets(longlong param_1,undefined8 *param_2)

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
    goto LAB_140293fdd;
  }
  pQVar5 = (QDateTime *)thunk_FUN_1402938b0(param_1 + 0x20,*(undefined4 *)(param_1 + 0x28));
  QDateTime::operator=(local_f8,pQVar5);
  QImage::operator=(local_f0,(QImage *)(pQVar5 + 8));
  local_d0 = pQVar5[0x28];
  *(int *)(param_1 + 0x28) = (*(int *)(param_1 + 0x28) + 1) % *(int *)(param_1 + 0x18);
  *(int *)(param_1 + 0x2c) = *(int *)(param_1 + 0x2c) + -1;
  local_res8 = operator_new(0x58);
  pQVar6 = pQVar13;
  if (local_res8 != (QArrayData *)0x0) {
    pQVar6 = (QArrayData *)thunk_FUN_140286ed0(local_res8);
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
      *(undefined1 **)(pQVar7 + 8) = &LAB_14000c27a;
      *(int *)(pQVar7 + 4) = 1;
      *(int *)pQVar7 = 1;
    }
    local_res8 = pQVar7;
    local_100 = pQVar7;
    thunk_FUN_14007cde0(pQVar7,pQVar6);
    FUN_140010983(&local_108,pQVar6);
  }
  QDateTime::operator=((QDateTime *)(pQVar6 + 0x30),local_f8);
  bVar1 = QImage::isNull(local_f0);
  pQVar14 = pQVar7;
  if (bVar1) {
    if (local_d0 != (QDateTime)0x0) {
      *(int *)(pQVar6 + 0x38) = 2;
      goto LAB_140293fa4;
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
LAB_140293f97:
    thunk_FUN_140294a50(param_1,ppQVar12,local_f0,uVar8);
  }
  else {
    FVar2 = QImage::format(local_f0);
    if (FVar2 != 0xb) {
      qt_assert("image.format() == QImage::Format_RGB555",
                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabber-video\\videoStreamSequencer.cpp"
                ,0x56);
    }
    bVar1 = QImage::isNull((QImage *)(param_1 + 0x68));
    if (bVar1) {
LAB_140293f58:
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
      goto LAB_140293f97;
    }
    iVar3 = QImage::width((QImage *)(param_1 + 0x68));
    iVar4 = QImage::width(local_f0);
    if (iVar3 != iVar4) goto LAB_140293f58;
    iVar3 = QImage::height((QImage *)(param_1 + 0x68));
    iVar4 = QImage::height(local_f0);
    if (iVar3 != iVar4) goto LAB_140293f58;
    thunk_FUN_140294700(&local_res8,local_f0,param_1 + 0x68);
    this = QArrayData::data(local_res8);
    pQVar9 = (QRect *)thunk_FUN_1402946c0(&local_res8);
    for (; this != pQVar9; this = this + 0x10) {
      iVar3 = QRect::height(this);
      iVar4 = QRect::width(this);
      uVar15 = (int)pQVar13 + iVar3 * iVar4 * 2 + 8;
      pQVar13 = (QArrayData *)(ulonglong)uVar15;
      param_2 = local_res10;
    }
    iVar3 = QImage::byteCount(local_f0);
    if ((float)(int)uVar15 / (float)(iVar3 + 8) <= _DAT_140450e90) {
      *(int *)(pQVar6 + 0x38) = 1;
      pvVar10 = QArrayData::data(local_res8);
      pvVar11 = (void *)thunk_FUN_1402946c0(&local_res8);
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
        thunk_FUN_140294a50(param_1,&local_a8,local_f0,pvVar10);
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
      thunk_FUN_140294a50(param_1,&local_b8,local_f0,uVar8);
    }
    pQVar13 = local_res8;
    if (*(int *)local_res8 == 0) {
LAB_140293f31:
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
      if (iVar3 == 1) goto LAB_140293f31;
    }
  }
LAB_140293fa4:
  QDateTime::operator=((QDateTime *)(param_1 + 0x38),local_f8);
  QImage::operator=((QImage *)(param_1 + 0x40),local_f0);
  *param_2 = pQVar6;
  param_2[1] = pQVar14;
  thunk_FUN_14028b1c0(0);
LAB_140293fdd:
  QImage::~QImage(local_f0);
  QDateTime::~QDateTime(local_f8);
  return param_2;
}
