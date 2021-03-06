#include <memory>
#include <string>
#include <vector>

#include "base_test.hpp"

#include "concurrency/transaction_context.hpp"
#include "hyrise.hpp"
#include "operators/get_table.hpp"
#include "operators/validate.hpp"
#include "storage/table.hpp"

namespace opossum {

class OperatorsValidateVisibilityTest : public BaseTest {
 protected:
  void SetUp() override {
    TableColumnDefinitions column_definitions;
    column_definitions.emplace_back("a", DataType::Int, false);
    column_definitions.emplace_back("b", DataType::Int, false);
    t = std::make_shared<Table>(column_definitions, TableType::Data, chunk_size, UseMvcc::Yes);
    t->append({123, 456});

    Hyrise::get().storage_manager.add_table(table_name, t);

    gt = std::make_shared<GetTable>(table_name);
    gt->execute();

    validate = std::make_shared<Validate>(gt);
  }

  std::string table_name = "validateTestTable";

  static constexpr auto chunk_size = uint32_t{10};

  std::shared_ptr<GetTable> gt;
  std::shared_ptr<Table> t;
  std::shared_ptr<Validate> validate;
};

// Legend:
// our_TID == row_TID, our_CID >= beg_CID, our_CID >= end_CID
// taken from: https://github.com/hyrise/hyrise/blob/master/docs/documentation/queryexecution/tx.rst

// yes, yes, yes
TEST_F(OperatorsValidateVisibilityTest, Impossible) {
  auto context = std::make_shared<TransactionContext>(2, 2, AutoCommit::No);

  t->get_chunk(ChunkID{0})->mvcc_data()->set_tid(0, 2);
  t->get_chunk(ChunkID{0})->mvcc_data()->set_begin_cid(0, 2);
  t->get_chunk(ChunkID{0})->mvcc_data()->set_end_cid(0, 2);

  validate->set_transaction_context(context);
  validate->execute();

  EXPECT_EQ(validate->get_output()->row_count(), 0u);
}

// no, yes, yes
TEST_F(OperatorsValidateVisibilityTest, PastDelete) {
  auto context = std::make_shared<TransactionContext>(2, 2, AutoCommit::No);

  t->get_chunk(ChunkID{0})->mvcc_data()->set_tid(0, 42);
  t->get_chunk(ChunkID{0})->mvcc_data()->set_begin_cid(0, 2);
  t->get_chunk(ChunkID{0})->mvcc_data()->set_end_cid(0, 2);

  validate->set_transaction_context(context);
  validate->execute();

  EXPECT_EQ(validate->get_output()->row_count(), 0u);
}

// yes, no, yes
TEST_F(OperatorsValidateVisibilityTest, Impossible2) {
  auto context = std::make_shared<TransactionContext>(2, 2, AutoCommit::No);

  t->get_chunk(ChunkID{0})->mvcc_data()->set_tid(0, 2);
  t->get_chunk(ChunkID{0})->mvcc_data()->set_begin_cid(0, 4);
  t->get_chunk(ChunkID{0})->mvcc_data()->set_end_cid(0, 1);

  validate->set_transaction_context(context);
  validate->execute();

  EXPECT_EQ(validate->get_output()->row_count(), 0u);
}

// yes, yes, no
TEST_F(OperatorsValidateVisibilityTest, OwnDeleteUncommitted) {
  auto context = std::make_shared<TransactionContext>(2, 2, AutoCommit::No);

  t->get_chunk(ChunkID{0})->mvcc_data()->set_tid(0, 2);
  t->get_chunk(ChunkID{0})->mvcc_data()->set_begin_cid(0, 1);
  t->get_chunk(ChunkID{0})->mvcc_data()->set_end_cid(0, 6);

  validate->set_transaction_context(context);
  validate->execute();

  EXPECT_EQ(validate->get_output()->row_count(), 0u);
}

// no, no, yes
TEST_F(OperatorsValidateVisibilityTest, Impossible3) {
  auto context = std::make_shared<TransactionContext>(2, 2, AutoCommit::No);

  t->get_chunk(ChunkID{0})->mvcc_data()->set_tid(0, 50);
  t->get_chunk(ChunkID{0})->mvcc_data()->set_begin_cid(0, 3);
  t->get_chunk(ChunkID{0})->mvcc_data()->set_end_cid(0, 1);

  validate->set_transaction_context(context);
  validate->execute();

  EXPECT_EQ(validate->get_output()->row_count(), 0u);
}

// yes, no, no
TEST_F(OperatorsValidateVisibilityTest, OwnInsert) {
  auto context = std::make_shared<TransactionContext>(2, 2, AutoCommit::No);

  t->get_chunk(ChunkID{0})->mvcc_data()->set_tid(0, 2);
  t->get_chunk(ChunkID{0})->mvcc_data()->set_begin_cid(0, 3);
  t->get_chunk(ChunkID{0})->mvcc_data()->set_end_cid(0, 3);

  validate->set_transaction_context(context);
  validate->execute();

  EXPECT_EQ(validate->get_output()->row_count(), 1u);
}

// no, yes, no
TEST_F(OperatorsValidateVisibilityTest, PastInsertOrFutureDelete) {
  auto context = std::make_shared<TransactionContext>(2, 2, AutoCommit::No);

  t->get_chunk(ChunkID{0})->mvcc_data()->set_tid(0, 99);
  t->get_chunk(ChunkID{0})->mvcc_data()->set_begin_cid(0, 2);
  t->get_chunk(ChunkID{0})->mvcc_data()->set_end_cid(0, 3);

  validate->set_transaction_context(context);
  validate->execute();

  EXPECT_EQ(validate->get_output()->row_count(), 1u);
}

// no, no, no
TEST_F(OperatorsValidateVisibilityTest, UncommittedInsertOrFutureInsert) {
  auto context = std::make_shared<TransactionContext>(2, 2, AutoCommit::No);

  t->get_chunk(ChunkID{0})->mvcc_data()->set_tid(0, 99);
  t->get_chunk(ChunkID{0})->mvcc_data()->set_begin_cid(0, 3);
  t->get_chunk(ChunkID{0})->mvcc_data()->set_end_cid(0, 3);

  validate->set_transaction_context(context);
  validate->execute();

  EXPECT_EQ(validate->get_output()->row_count(), 0u);
}

}  // namespace opossum
