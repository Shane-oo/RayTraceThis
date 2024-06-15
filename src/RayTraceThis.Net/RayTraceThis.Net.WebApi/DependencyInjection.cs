using System.Reflection;
using RayTraceThis.Net.Identity.Users;

namespace RayTraceThis.Net.WebApi;

public static class DependencyInjection
{
    #region Private Methods

    private static Assembly[] GetAssemblies()
    {
        return
        [
            typeof(DependencyInjection).Assembly, // RayTraceThis.Net.WebApi
            typeof(UsersModule).Assembly // RayTraceThis.Net.Identity
        ];
    }

    #endregion

    #region Public Methods

    public static void AddDataContext(this IServiceCollection services, IConfiguration configuration)
    {
        //todo
    }

    public static void AddDbQueries(this IServiceCollection services)
    {
        //todo
    }

    public static void AddFluentValidators(this IServiceCollection services)
    {
        //todo
    }

    public static void AddMediatRServices(this IServiceCollection services)
    {
        services.AddMediatR(config =>
                            {
                                config.RegisterServicesFromAssemblies(GetAssemblies());

                                //config.AddOpenBehavior(typeof(FluentValidationBehaviour<,>));
                            });
    }

    public static void AddOpenIddictServer(this IServiceCollection services, IWebHostEnvironment environment)
    {
        //todo
    }

    public static void AddUserIdentity(this IServiceCollection services)
    {
        //todo
    }

    #endregion
}
