// DIContainerTest.cpp
#if WITH_AUTOMATION_TESTS
#include "DIContainerTest.h"
#include "Engine/Engine.h"
#include "DIContainerModule/Public/DIContainer/DIContainer.h"
#include "Scoped/LogErrorSuppression.h"
#include "Scoped/LogErrorSuppression.h"

// Helper functions (outside the test class)
namespace DIContainerTestHelpers
{
    auto ResetContainer()
    {
        return NewObject<UDIContainer>();
    }
    
    void TestBasicRegistrationAndResolution(FAutomationTestBase* Test)
    {
        UDIContainer* Container = ResetContainer();
        
        // Create test instances
        UTestBaseService* BaseService = NewObject<UTestBaseService>();
        UTestAnotherService* AnotherService = NewObject<UTestAnotherService>();

        // Test template registration
        Container->Register<UTestBaseService>(BaseService);
        Container->Register<UTestAnotherService>(AnotherService);

        // Test template resolution
        UTestBaseService* ResolvedBase = Container->Resolve<UTestBaseService>();
        UTestAnotherService* ResolvedAnother = Container->Resolve<UTestAnotherService>();

        Test->TestNotNull("Base service should be resolved", ResolvedBase);
        Test->TestNotNull("Another service should be resolved", ResolvedAnother);
        Test->TestEqual("Resolved base service should be same instance", ResolvedBase, BaseService);
        Test->TestEqual("Resolved another service should be same instance", ResolvedAnother, AnotherService);
    }

    void TestNamedRegistrationAndResolution(FAutomationTestBase* Test)
    {
        UDIContainer* Container = ResetContainer();
        
        // Create multiple instances of the same type
        UTestBaseService* Service1 = NewObject<UTestBaseService>();
        UTestBaseService* Service2 = NewObject<UTestBaseService>();

        FName Service1Name = TEXT("Service1");
        FName Service2Name = TEXT("Service2");

        // Register with names using templates
        Container->Register<UTestBaseService>(Service1Name, Service1);
        Container->Register<UTestBaseService>(Service2Name, Service2);

        // Resolve with names using templates
        UTestBaseService* ResolvedService1 = Container->Resolve<UTestBaseService>(Service1Name);
        UTestBaseService* ResolvedService2 = Container->Resolve<UTestBaseService>(Service2Name);

        Test->TestNotNull("Service1 should be resolved", ResolvedService1);
        Test->TestNotNull("Service2 should be resolved", ResolvedService2);
        Test->TestEqual("Resolved Service1 should be correct instance", ResolvedService1, Service1);
        Test->TestEqual("Resolved Service2 should be correct instance", ResolvedService2, Service2);
        Test->TestNotEqual("Services should be different instances", ResolvedService1, ResolvedService2);
    }

    void TestInheritanceScenarios(FAutomationTestBase* Test)
    {
        // Create an empty container for a clean test.
        UDIContainer* Container = ResetContainer();
        
        // Create derived service instance
        UTestDerivedService* DerivedService = NewObject<UTestDerivedService>();

        // Register derived service as base type
        Container->Register<UTestBaseService>(DerivedService);

        // Resolve as base type
        UTestBaseService* ResolvedAsBase = Container->Resolve<UTestBaseService>();
        Test->TestNotNull("Derived service should be resolved as base type", ResolvedAsBase);
        Test->TestTrue("Resolved service should be the derived instance", ResolvedAsBase == DerivedService);

        // Verify polymorphic behavior
        FString ServiceName = ResolvedAsBase->GetServiceName();
        Test->TestEqual("Polymorphic method should return derived implementation", ServiceName, TEXT("DerivedService"));
    }

    void TestAbstractionScenarios(FAutomationTestBase* Test)
    {
        // Create abstractio service
        UTestAbstractedService* AbstractionService = NewObject<UTestAbstractedService>();
        
        // First, register as BP interface, using native method
        UDIContainer* Container = ResetContainer();
        Container->Register<ITestAbstraction>(AbstractionService);
        auto BPResolvedTest = Container->Resolve<ITestAbstraction>();
        Test->TestNotNull("Resolved service should not be null", BPResolvedTest);
        Test->TestTrue("Resolved abstraction of service should be casted to UTestAbstractedService", static_cast<bool>(Cast<UTestAbstractedService>(BPResolvedTest)));
        Test->TestTrue("Function call mustve returned true", BPResolvedTest->DoSomething());
    }

    void TestErrorHandling(FAutomationTestBase* Test)
    {
        // Create a new container for clean test
        UDIContainer* Container = ResetContainer();
        
        // Test registering null instance
        Container->Register(UTestBaseService::StaticClass(), nullptr);
        // Should log error and not crash

        // Test resolving non-registered type
        UObject* NonExistentService = Container->Resolve(UTestAnotherService::StaticClass());
        Test->TestNull("Non-registered service should return null", NonExistentService);

        // Test resolving with null class
        UObject* NullClassResult = Container->Resolve(nullptr);
        Test->TestNull("Resolving with null class should return null", NullClassResult);

        // Test double registration (should log error but not crash)
        UTestBaseService* Service = NewObject<UTestBaseService>();
        Container->Register<UTestBaseService>(Service);
        Container->Register<UTestBaseService>(Service); // Should log error
    }

    void TestUnregistration(FAutomationTestBase* Test)
    {
        // Create a new container for cleaner test
        UDIContainer* Container = ResetContainer();
        
        // Create and register services
        UTestBaseService* ServiceToRemove = NewObject<UTestBaseService>();
        UTestAnotherService* ServiceToKeep = NewObject<UTestAnotherService>();

        Container->Register<UTestBaseService>(ServiceToRemove);
        Container->Register<UTestAnotherService>(ServiceToKeep);

        // Verify they're registered
        Test->TestNotNull("Service to remove should be resolvable before unregistration", 
                         Container->Resolve<UTestBaseService>());
        Test->TestNotNull("Service to keep should be resolvable before unregistration", 
                         Container->Resolve<UTestAnotherService>());

        // Unregister one service
        Container->UnregisterByInstance(ServiceToRemove);

        // Verify unregistration
        Test->TestNull("Service should not be resolvable after unregistration", 
                      Container->Resolve<UTestBaseService>());
        Test->TestNotNull("Other service should still be resolvable after partial unregistration", 
                         Container->Resolve<UTestAnotherService>());

        // Test unregistering non-existent service (should not crash)
        UTestBaseService* NonRegisteredService = NewObject<UTestBaseService>();
        Container->UnregisterByInstance(NonRegisteredService); // Should not crash
    }
}

// Main test class using the macro
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDIContainerTest, "Game.DIContainer.BasicFunctionality", 
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FDIContainerTest::RunTest(const FString& Parameters)
{
    // Create DI Container
    UDIContainer* Container = DIContainerTestHelpers::ResetContainer();
    TestNotNull("DI Container should be created", Container);

    FScopedLogErrorSuppression LogErrorSuppression(this); // Suppress error for this test
    
    // Test basic registration and resolution
    DIContainerTestHelpers::TestBasicRegistrationAndResolution(this);
    
    // Test named registration and resolution
    DIContainerTestHelpers::TestNamedRegistrationAndResolution(this);

    // Test inheritance scenarios
    DIContainerTestHelpers::TestInheritanceScenarios(this);
    
    // Test error handling
    DIContainerTestHelpers::TestErrorHandling(this);
    
    // Test unregistration
    DIContainerTestHelpers::TestUnregistration(this);

    return true;
}

#endif // WITH_AUTOMATION_TESTS