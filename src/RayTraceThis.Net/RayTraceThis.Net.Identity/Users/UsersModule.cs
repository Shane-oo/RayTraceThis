using Carter;
using JetBrains.Annotations;
using MediatR;
using Microsoft.AspNetCore.Builder;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Routing;
using RayTraceThis.Net.Identity.Users.GetUsers;

namespace RayTraceThis.Net.Identity.Users;

[UsedImplicitly]
public class UsersModule: CarterModule
{
    #region Construction

    public UsersModule(): base("api/users")
    {
    }

    #endregion

    #region Public Methods

    public override void AddRoutes(IEndpointRouteBuilder app)
    {
        app.MapGet("/", async ([FromBody] GetUsersRequest request, ISender sender, CancellationToken cancellationToken) =>
                        {
                            var query = new GetUsersQuery();
                            var result = await sender.Send(query, cancellationToken);

                            return result.IsFailure ? Results.BadRequest(result.ErrorResult) : Results.Ok(result.Value);
                        });
    }

    #endregion
}
