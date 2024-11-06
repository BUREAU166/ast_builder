#include "../include/metrics.h"
#include <iostream>

Metrics::Metrics(const std::vector<std::string>& files) {
    CXIndex index = clang_createIndex(0, 0);

    for (const auto& file : files) {
        MetricsData fileMetrics;
        CXTranslationUnit unit = clang_parseTranslationUnit(index, file.c_str(), nullptr, 0, nullptr, 0, CXTranslationUnit_None);
        if (unit == nullptr) {
            std::cerr << "Error parsing file: " << file << std::endl;
            continue;
        }

        CXCursor rootCursor = clang_getTranslationUnitCursor(unit);
        clang_visitChildren(rootCursor, [](CXCursor c, CXCursor parent, CXClientData clientData) {
            MetricsData* data = static_cast<MetricsData*>(clientData);
            collectMetrics(c, *data);
            return CXChildVisit_Recurse;
        }, &fileMetrics);

        fileMetricsMap[file] = fileMetrics;
        aggregateMetrics(fileMetrics);
        clang_disposeTranslationUnit(unit);
    }

    clang_disposeIndex(index);
}

const Metrics::MetricsData& Metrics::getMetrics() const {
    return metrics;
}

void Metrics::printMetrics() const {
    std::cout << "Code Metrics Summary (All Files):\n";
    printMetricsData(metrics);

    for (const auto& [file, fileMetrics] : fileMetricsMap) {
        std::cout << "\nMetrics for file: " << file << "\n";
        printMetricsData(fileMetrics);
    }
}

void Metrics::collectMetrics(CXCursor cursor, MetricsData& data) {
    switch (clang_getCursorKind(cursor)) {
        case CXCursor_ClassDecl:
            data.classCount++;
            break;
        case CXCursor_CXXMethod:
        case CXCursor_FunctionDecl:
            data.methodCount++;
            break;
        case CXCursor_IfStmt:
            data.ifCount++;
            break;
        case CXCursor_ForStmt:
            data.forCount++;
            break;
        case CXCursor_WhileStmt:
            data.whileCount++;
            break;
        case CXCursor_SwitchStmt:
            data.switchCount++;
            break;
        case CXCursor_CaseStmt:
            data.caseCount++;
            break;
        case CXCursor_VarDecl:
            data.variableCount++;
            break;
        case CXCursor_PreprocessingDirective: // Rough count for comments
            data.commentCount++;
            break;
        default:
            break;
    }
}

void Metrics::aggregateMetrics(const MetricsData& fileMetrics) {
    metrics.classCount += fileMetrics.classCount;
    metrics.methodCount += fileMetrics.methodCount;
    metrics.ifCount += fileMetrics.ifCount;
    metrics.forCount += fileMetrics.forCount;
    metrics.whileCount += fileMetrics.whileCount;
    metrics.switchCount += fileMetrics.switchCount;
    metrics.caseCount += fileMetrics.caseCount;
    metrics.variableCount += fileMetrics.variableCount;
    metrics.commentCount += fileMetrics.commentCount;
}

void Metrics::printMetricsData(const MetricsData& data) const {
    std::cout << "Number of classes: " << data.classCount << "\n";
    std::cout << "Number of methods: " << data.methodCount << "\n";
    std::cout << "Number of if statements: " << data.ifCount << "\n";
    std::cout << "Number of for loops: " << data.forCount << "\n";
    std::cout << "Number of while loops: " << data.whileCount << "\n";
    std::cout << "Number of switch statements: " << data.switchCount << "\n";
    std::cout << "Number of case statements: " << data.caseCount << "\n";
    std::cout << "Number of variable declarations: " << data.variableCount << "\n";
    std::cout << "Approximate number of comments: " << data.commentCount << "\n";
}
