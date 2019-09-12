#include "constant_mappings.hpp"

#include <boost/bimap.hpp>
#include <boost/hana/fold.hpp>

#include <string>
#include <unordered_map>

#include "sql/Expr.h"
#include "sql/SelectStatement.h"

#include "expression/abstract_expression.hpp"
#include "expression/aggregate_expression.hpp"
#include "storage/encoding_type.hpp"
#include "storage/table.hpp"
#include "storage/vector_compression/vector_compression.hpp"
#include "utils/make_bimap.hpp"

namespace opossum {

const std::unordered_map<OperatorType, std::string> operator_type_to_string = {
    {OperatorType::Aggregate, "Aggregate"},
    {OperatorType::Alias, "Alias"},
    {OperatorType::Delete, "Delete"},
    {OperatorType::Difference, "Difference"},
    {OperatorType::ExportBinary, "ExportBinary"},
    {OperatorType::ExportCsv, "ExportCsv"},
    {OperatorType::GetTable, "GetTable"},
    {OperatorType::ImportBinary, "ImportBinary"},
    {OperatorType::ImportCsv, "ImportCsv"},
    {OperatorType::IndexScan, "IndexScan"},
    {OperatorType::Insert, "Insert"},
    {OperatorType::JitOperatorWrapper, "JitOperatorWrapper"},
    {OperatorType::JoinHash, "JoinHash"},
    {OperatorType::JoinIndex, "JoinIndex"},
    {OperatorType::JoinMPSM, "JoinMPSM"},
    {OperatorType::JoinNestedLoop, "JoinNestedLoop"},
    {OperatorType::JoinSortMerge, "JoinSortMerge"},
    {OperatorType::Limit, "Limit"},
    {OperatorType::Print, "Print"},
    {OperatorType::Product, "Product"},
    {OperatorType::Projection, "Projection"},
    {OperatorType::Sort, "Sort"},
    {OperatorType::TableScan, "TableScan"},
    {OperatorType::TableWrapper, "TableWrapper"},
    {OperatorType::UnionAll, "UnionAll"},
    {OperatorType::UnionPositions, "UnionPositions"},
    {OperatorType::Update, "Update"},
    {OperatorType::Validate, "Validate"},
    {OperatorType::CreateTable, "CreateTable"},
    {OperatorType::CreateView, "CreateView"},
    {OperatorType::DropTable, "DropTable"},
    {OperatorType::DropView, "DropView"},
    {OperatorType::Mock, "Mock"}};

const boost::bimap<AggregateFunction, std::string> aggregate_function_to_string =
    make_bimap<AggregateFunction, std::string>({
        {AggregateFunction::Min, "MIN"},
        {AggregateFunction::Max, "MAX"},
        {AggregateFunction::Sum, "SUM"},
        {AggregateFunction::Avg, "AVG"},
        {AggregateFunction::Count, "COUNT"},
        {AggregateFunction::CountDistinct, "COUNT DISTINCT"},
        {AggregateFunction::StandardDeviationSample, "STDDEV_SAMP"},
    });

const boost::bimap<FunctionType, std::string> function_type_to_string =
    make_bimap<FunctionType, std::string>({{FunctionType::Substring, "SUBSTR"}, {FunctionType::Concatenate, "CONCAT"}});

const boost::bimap<DataType, std::string> data_type_to_string =
    hana::fold(data_type_enum_string_pairs, boost::bimap<DataType, std::string>{}, [](auto map, auto pair) {
      map.insert({hana::first(pair), std::string{hana::second(pair)}});
      return map;
    });

const boost::bimap<EncodingType, std::string> encoding_type_to_string = make_bimap<EncodingType, std::string>({
    {EncodingType::Dictionary, "Dictionary"},
    {EncodingType::RunLength, "RunLength"},
    {EncodingType::FixedStringDictionary, "FixedStringDictionary"},
    {EncodingType::FrameOfReference, "FrameOfReference"},
    {EncodingType::LZ4, "LZ4"},
    {EncodingType::Unencoded, "Unencoded"},
});

const boost::bimap<VectorCompressionType, std::string> vector_compression_type_to_string =
    make_bimap<VectorCompressionType, std::string>({
        {VectorCompressionType::FixedSizeByteAligned, "Fixed-size byte-aligned"},
        {VectorCompressionType::SimdBp128, "SIMD-BP128"},
    });

std::ostream& operator<<(std::ostream& stream, AggregateFunction aggregate_function) {
  return stream << aggregate_function_to_string.left.at(aggregate_function);
}

std::ostream& operator<<(std::ostream& stream, FunctionType function_type) {
  return stream << function_type_to_string.left.at(function_type);
}

std::ostream& operator<<(std::ostream& stream, DataType data_type) {
  return stream << data_type_to_string.left.at(data_type);
}

std::ostream& operator<<(std::ostream& stream, EncodingType encoding_type) {
  return stream << encoding_type_to_string.left.at(encoding_type);
}

std::ostream& operator<<(std::ostream& stream, VectorCompressionType vector_compression_type) {
  return stream << vector_compression_type_to_string.left.at(vector_compression_type);
}

}  // namespace opossum
