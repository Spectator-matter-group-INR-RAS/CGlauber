#include <CGlauberFactory.hh>
#include <COLA.hh>
#include <gtest/gtest.h>

#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>

extern "C" cola::VModule* LoadCOLAModule();

namespace {

  class TestWriter final : public cola::VWriter {
   public:
    void operator()(std::unique_ptr<cola::EventData>&& /*data*/) override {}
  };

  class TestWriterFactory final : public cola::VWriterFactory {
   public:
    std::unique_ptr<cola::VFilter> Create(const std::unordered_map<std::string, std::string>& /*meta_data*/) override {
      return std::make_unique<TestWriter>();
    }

    const std::string& GetFilterName() const override {
      static const std::string name{"test_writer"};
      return name;
    }
  };

  std::unordered_map<std::string, std::string> DefaultGlauberParams() {
    return {
        {"name_a", "Au"}, {"name_b", "Au"}, {"energy", "200"}, {"is_collider", "1"}, {"fermiMom", "1"},
    };
  }

}  // namespace

TEST(CGlauberTest, ModuleExposesGeneratorFactory) {
  const auto module = std::unique_ptr<cola::VModule>(LoadCOLAModule());
  const cola::FactoryMap factories = module->GetModuleFilters();

  ASSERT_NE(factories.find("CGlauber"), factories.end());
  EXPECT_EQ(factories.at("CGlauber")->GetFilterType(), cola::FilterType::kGenerator);
  EXPECT_EQ(factories.at("CGlauber")->GetFilterName(), "CGlauber");
}

TEST(CGlauberTest, FactoryCreatesGenerator) {
  cola::CGlauberFactory factory;
  const auto filter = factory.Create(DefaultGlauberParams());
  ASSERT_NE(filter, nullptr);
  EXPECT_EQ(factory.GetFilterName(), "CGlauber");
}

TEST(CGlauberTest, GeneratorProducesNonEmptyEvent) {
  cola::CGlauberFactory factory;
  const auto filter = factory.Create(DefaultGlauberParams());
  auto* generator = dynamic_cast<cola::VGenerator*>(filter.get());
  ASSERT_NE(generator, nullptr);

  const auto event = (*generator)();
  ASSERT_NE(event, nullptr);
  EXPECT_FALSE(event->particles.empty());
  EXPECT_FALSE(event->ini_state.ini_state_particles.empty());
}

TEST(CGlauberTest, ParseAndRunViaMetaProcessor) {
  const std::string xml = R"(<?xml version="1.0"?>
<program>
    <generator name="CGlauber" name_a="Au" name_b="Au" energy="200" is_collider="1" fermiMom="1"/>
    <writer name="test_writer"/>
</program>
)";

  const auto module = std::unique_ptr<cola::VModule>(LoadCOLAModule());
  cola::MetaProcessor processor(module->GetModuleFilters());
  processor.Register(std::make_unique<TestWriterFactory>());
  std::istringstream stream(xml);
  cola::FilterEnsemble ensemble = processor.Parse(stream);

  ASSERT_NE(ensemble.generator, nullptr);
  const auto event = (*ensemble.generator)();
  ASSERT_NE(event, nullptr);
  EXPECT_FALSE(event->particles.empty());
}
