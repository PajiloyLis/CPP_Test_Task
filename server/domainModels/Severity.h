#ifndef SERVER_SEVERITY_H
#define SERVER_SEVERITY_H

enum class Severity {
    Info,
    Warning,
    Error,
    Unknown
};

inline QString severityToString(Severity s) {
    switch (s) {
        case Severity::Info:    return QStringLiteral("INFO");
        case Severity::Warning: return QStringLiteral("WARN");
        case Severity::Error:   return QStringLiteral("ERROR");
        case Severity::Unknown: return QStringLiteral("UNKNOWN");
    }
    return QStringLiteral("UNKNOWN");
}

inline Severity severityFromString(const QString& s) {
    const QString t = s.trimmed().toUpper();
    if (t == QStringLiteral("INFO"))    return Severity::Info;
    if (t == QStringLiteral("WARN"))    return Severity::Warning;
    if (t == QStringLiteral("WARNING")) return Severity::Warning;
    if (t == QStringLiteral("ERROR"))   return Severity::Error;
    return Severity::Unknown;
}

#endif //SERVER_SEVERITY_H
