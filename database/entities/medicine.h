#ifndef MEDICINE_H
#define MEDICINE_H

#include <QString>
#include <QDate>

class Medicine
{
public:
    Medicine();

    // Getter方法
    int id() const { return m_id; }
    QString name() const { return m_name; }
    QString category() const { return m_category; }
    int stock() const { return m_stock; }
    int warningThreshold() const { return m_warningThreshold; }
    double unitPrice() const { return m_unitPrice; }
    QDate expiryDate() const { return m_expiryDate; }

    // Setter方法
    void setId(int id) { m_id = id; }
    void setName(const QString& name) { m_name = name; }
    void setCategory(const QString& category) { m_category = category; }
    void setStock(int stock) { m_stock = stock; }
    void setWarningThreshold(int warningThreshold) { m_warningThreshold = warningThreshold; }
    void setUnitPrice(double unitPrice) { m_unitPrice = unitPrice; }
    void setExpiryDate(const QDate& expiryDate) { m_expiryDate = expiryDate; }

    // 数据验证
    bool isValid() const;

    // 库存预警检查
    bool isBelowWarningThreshold() const;

private:
    int m_id;
    QString m_name;
    QString m_category;
    int m_stock;
    int m_warningThreshold;
    double m_unitPrice;
    QDate m_expiryDate;
};

#endif // MEDICINE_H