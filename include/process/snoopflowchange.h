// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_FLOW_CHANGE_H__
#define __SNOOP_FLOW_CHANGE_H__

#include <SnoopProcess>
#include <SnoopFlowMgr>
#ifdef QT_GUI_LIB
#include <QTreeWidget>
#include <QHeaderView>
#include <VListWidget>
#endif // QT_GUI_LIB

// ----------------------------------------------------------------------------
// SnoopFlowChangeItem
// ----------------------------------------------------------------------------
class SnoopFlowChangeItem : public VXmlable
{
public:
  enum Protocol {
    Tcp,
    Udp
  };

  enum IpChangeType {
    IpCopy,
    IpFix
  };

  enum PortChangeType {
    PortCopy,
    PortAutoInc,
    PortFix
  };

public:
  SnoopFlowChangeItem();

public:
  bool       enabled;
  bool       log;

  Protocol   protocol;

  Ip             srcIp;
  Ip             srcIpMask;
  IpChangeType   srcIpChangeType;
  Ip             srcIpFixValue;

  UINT16         srcPort;
  PortChangeType srcPortChangeType;
  UINT16         srcPortFixValue;

  Ip             dstIp;
  Ip             dstIpMask;
  IpChangeType   dstIpChangeType;
  Ip             dstIpFixValue;

  UINT16         dstPort;
  PortChangeType dstPortChangeType;
  UINT16         dstPortFixValue;

public:
  bool prepare(VError& error);
  bool check(SnoopTransportFlowKey& flowKey);

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);

#ifdef QT_GUI_LIB
public:
  static const int ENABLED_IDX          = 0;
  static const int LOG_IDX              = 1;
  static const int PROTOCOL_IDX         = 2;

  static const int SRC_IP_IDX           = 3;
  static const int SRC_IP_MASK_IDX      = 4;
  static const int SRC_IP_CHANGE_TYPE   = 5;
  static const int SRC_IP_FIX_VALUE     = 6;

  static const int SRC_PORT_IDX         = 7;
  static const int SRC_PORT_CHANGE_TYPE = 8;
  static const int SRC_PORT_FIX_VALUE   = 9;

  static const int DST_IP_IDX           = 10;
  static const int DST_IP_MASK_IDX      = 11;
  static const int DST_IP_CHANGE_TYPE   = 12;
  static const int DST_IP_FIX_VALUE     = 13;

  static const int DST_PORT_IDX         = 14;
  static const int DST_PORT_CHANGE_TYPE = 15;
  static const int DST_PORT_FIX_VALUE   = 16;

public:
  static void initialize(QTreeWidget* treeWidget);
#endif // QT_GUI_LIB
};

#ifdef QT_GUI_LIB
void operator << (QTreeWidgetItem& treeWidgetItem, SnoopFlowChangeItem& item);
void operator << (SnoopFlowChangeItem& item, QTreeWidgetItem& treeWidgetItem);
#endif // QT_GUI_LIB

// ----------------------------------------------------------------------------
// SnoopFlowChangeItems
// ----------------------------------------------------------------------------
class SnoopFlowChangeItems : public QObject, public QList<SnoopFlowChangeItem>, public VXmlable, public VOptionable, public VListWidgetAccessible
{
  Q_OBJECT

public:
  SnoopFlowChangeItems();
  virtual ~SnoopFlowChangeItems();

protected:
  UINT16 lastAutoIncSrcPort;
  UINT16 lastAutoIncDstPort;

public:
  bool prepare(VError& error);
  SnoopFlowChangeItem*  find(SnoopTransportFlowKey& flowKey);
  SnoopTransportFlowKey change(SnoopFlowChangeItem& item, SnoopTransportFlowKey& flowKey);

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);

#ifdef QT_GUI_LIB
public: // VOptionable
  virtual void optionAddWidget(QLayout* layout);
  virtual void optionSaveDlg(QDialog* dialog);

public: // VListWidgetAccessible
  virtual void  widgetClear()                                                             { clear();                                        }
  virtual int   widgetCount()                                                             { return count();                                 }
  virtual void* widgetAt(int i)                                                           { return  (void*)&at(i);                          }
  virtual void  widgetPushBack(void* item)                                                { push_back(*(const SnoopFlowChangeItem*)item);   }
  virtual void* widgetCeateItem()                                                         { return new SnoopFlowChangeItem;                 }
  virtual void  widgetItemIntoTreeWidgetItem(void* item, QTreeWidgetItem* treeWidgetItem) { *treeWidgetItem << *(SnoopFlowChangeItem*)item; }
  virtual void  widgetTreeWidgetItemIntoItem(QTreeWidgetItem* treeWidgetItem, void* item) { *(SnoopFlowChangeItem*)item << *treeWidgetItem; }
#endif // QT_GUI_LIB
};

// ----------------------------------------------------------------------------
// SnoopFlowChangeFlowItem
// ----------------------------------------------------------------------------
class SnoopFlowChangeFlowItem
{
public:
  bool                  changed;
  SnoopTransportFlowKey from;
  SnoopTransportFlowKey to;
};

// ----------------------------------------------------------------------------
// SnoopFlowChange
// ----------------------------------------------------------------------------
class SnoopFlowChange : public SnoopProcess
{
  Q_OBJECT

public:
  SnoopFlowChange(void* owner = NULL);
  virtual ~SnoopFlowChange();

protected:
  virtual bool doOpen();
  virtual bool doClose();

signals:
  void changed(SnoopPacket* packet);
  void unchanged(SnoopPacket* packet);

public slots:
  void processInOut(SnoopPacket* packet);
  void processOutIn(SnoopPacket* packet);

public:
  SnoopFlowMgr*        flowMgr;
  bool                 tcpChange;
  bool                 udpChange;
  SnoopFlowChangeItems changeItems;

protected:
    size_t                     tcpFlowOffset;
    size_t                     udpFlowOffset;

protected slots:
  void __tcpFlowCreate(SnoopTcpFlowKey* key, SnoopFlowValue* value);
  void __tcpFlowDelete(SnoopTcpFlowKey* key, SnoopFlowValue* value);
  void __udpFlowCreate(SnoopUdpFlowKey* key, SnoopFlowValue* value);
  void __udpFlowDelete(SnoopUdpFlowKey* key, SnoopFlowValue* value);

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);

#ifdef QT_GUI_LIB
public:
  virtual void optionAddWidget(QLayout* layout);
  virtual void optionSaveDlg(QDialog* dialog);
#endif // QT_GUI_LIB
};
#endif // __SNOOP_FLOW_CHANGE_H__