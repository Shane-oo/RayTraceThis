using RayTraceThis.Net.Core.Exchange;

namespace RayTraceThis.Net.Identity.Users.GetUsers;

public class GetUsersQueryHandler: IQueryHandler<GetUsersQuery, List<UsersListModel>>
{
    #region Public Methods

    public async Task<Result<List<UsersListModel>>> Handle(GetUsersQuery query, CancellationToken cancellationToken)
    {
        if (query is null)
        {
            return Error.NullValue;
        }

        return new List<UsersListModel>
               {
                   new()
                   {
                       Name = "Shane"
                   },
                   new()
                   {
                       Name = "Bob"
                   }
               };
    }

    #endregion
}
