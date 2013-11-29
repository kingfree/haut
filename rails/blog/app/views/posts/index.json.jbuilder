json.array!(@posts) do |post|
  json.extract! post, :title, :state, :text
  json.url post_url(post, format: :json)
end
